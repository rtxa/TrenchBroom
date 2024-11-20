/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ReadGeBmpTexture.h"

#include "Ensure.h"
#include "FreeImage.h"
#include "ReadFreeImageTexture.h"
#include "io/DiskFileSystem.h"
#include "io/ImageLoaderImpl.h"
#include "io/MaterialUtils.h"
#include "io/Reader.h"
#include "mdl/Texture.h"
#include "mdl/TextureBuffer.h"

#include "kdl/resource.h"
#include "kdl/string_utils.h"
#include "kdl/vector_utils.h"

#include <GenesisSDK/bitmap.h>
#include <GenesisSDK/genesis.h>
#include <GenesisSDK/ram.h>
#include <fmt/format.h>

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

namespace tb::io
{

namespace
{
/**
 * The byte order of a 32bpp FIBITMAP is defined by the macros FI_RGBA_RED,
 * FI_RGBA_GREEN, FI_RGBA_BLUE, FI_RGBA_ALPHA.
 * From looking at FreeImage.h, there are only two possible orders,
 * so we can handle both possible orders and map them to the relevant GL_RGBA
 * or GL_BGRA constant.
 */
constexpr GLenum freeImage32BPPFormatToGLFormat()
{
  if constexpr (
    FI_RGBA_RED == 0 && FI_RGBA_GREEN == 1 && FI_RGBA_BLUE == 2 && FI_RGBA_ALPHA == 3)
  {

    return GL_RGBA;
  }

  if constexpr (
    FI_RGBA_BLUE == 0 && FI_RGBA_GREEN == 1 && FI_RGBA_RED == 2 && FI_RGBA_ALPHA == 3)
  {

    return GL_BGRA;
  }

  throw std::runtime_error{"Expected FreeImage to use RGBA or BGRA"};
}

Result<mdl::Texture> createTextureFromFreeImageBitmap(FIBITMAP* image)
{
  try
  {
    InitFreeImage::initialize();

    if (!image)
    {
      return Error{"Invalid FreeImage bitmap provided"};
    }

    const auto imageWidth = size_t(FreeImage_GetWidth(image));
    const auto imageHeight = size_t(FreeImage_GetHeight(image));

    if (!checkTextureDimensions(imageWidth, imageHeight))
    {
      return Error{
        fmt::format("Invalid texture dimensions: {}*{}", imageWidth, imageHeight)};
    }

    // Check if the image has transparency
    const auto masked = FreeImage_IsTransparent(image);

    constexpr auto mipCount = 1u;
    constexpr auto format = freeImage32BPPFormatToGLFormat();

    auto buffers = mdl::TextureBufferList{mipCount};
    mdl::setMipBufferSize(buffers, mipCount, imageWidth, imageHeight, format);

    // Ensure the image is in 32-bit format
    if (
      FreeImage_GetColorType(image) != FIC_RGBALPHA
      || FreeImage_GetLine(image) / FreeImage_GetWidth(image) != 4)
    {
      image = FreeImage_ConvertTo32Bits(image);
      if (!image)
      {
        return Error{"Unsupported pixel format"};
      }
    }

    assert(FreeImage_GetLine(image) / FreeImage_GetWidth(image) == 4);

    auto* outBytes = buffers.at(0).data();
    const auto outBytesPerRow = int(imageWidth * 4);

    FreeImage_ConvertToRawBits(
      outBytes,
      image,
      outBytesPerRow,
      32,
      FI_RGBA_RED_MASK,
      FI_RGBA_GREEN_MASK,
      FI_RGBA_BLUE_MASK,
      TRUE);

    const auto textureMask = masked ? mdl::TextureMask::On : mdl::TextureMask::Off;
    const auto averageColor = getAverageColor(buffers.at(0), format);

    return mdl::Texture{
      imageWidth,
      imageHeight,
      averageColor,
      format,
      textureMask,
      mdl::NoEmbeddedDefaults{},
      std::move(buffers)};
  }
  catch (const std::exception& e)
  {
    return Error{e.what()};
  }
}

auto loadPlaceHolderTexture(const std::string& name)
{
  auto diskFS =
    DiskFileSystem{std::filesystem::current_path() / "defaults/assets/textures/"};

  const auto file = diskFS.openFile(name) | kdl::value();
  auto reader = file->reader().buffer();
  return readFreeImageTexture(reader);
}

Result<kdl::resource<FIBITMAP*>> createFreeImageFromGeBitmap8Bit(geBitmap* pBitmap)
{
  // Lock the geBitmap for reading
  geBitmap* pLock = nullptr;
  if (!geBitmap_LockForRead(pBitmap, &pLock, 0, 0, GE_PIXELFORMAT_8BIT, false, 0))
  {
    return Error{"Error locking geBitmap for reading."};
  }

  geBitmap_Info bitmapInfo;
  geBitmap_GetInfo(pLock, &bitmapInfo, nullptr);
  auto width = bitmapInfo.Width;
  auto height = bitmapInfo.Height;

  // Create a FreeImage bitmap of 32 bits to simplify the conversion
  InitFreeImage::initialize();
  auto fiBitmap = kdl::resource{FreeImage_Allocate(width, height, 32), FreeImage_Unload};
  if (!fiBitmap)
  {
    geBitmap_UnLock(pLock);
    return Error{"Failed to create bitmap"};
  }

  // Fill the palette
  RGBQUAD palette[256];
  for (int i = 0; i < 256; i++)
  {
    int r, g, b, a;
    geBitmap_Palette_GetEntryColor(bitmapInfo.Palette, i, &r, &g, &b, &a);
    palette[i].rgbRed = (BYTE)r;
    palette[i].rgbGreen = (BYTE)g;
    palette[i].rgbBlue = (BYTE)b;
    palette[i].rgbReserved = (BYTE)a;
  }

  auto* pPixelData = (BYTE*)geBitmap_GetBits(pLock);

  // Copy pixel data to FreeImage bitmap using the palette
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      // Get the palette index for the current pixel
      auto index = pPixelData[y * bitmapInfo.Width + x];

      // Get the corresponding color from the palette
      auto pixel = palette[index];

      // Set the pixel color in the FreeImage bitmap
      FreeImage_SetPixelColor(
        *fiBitmap,
        static_cast<uint32_t>(x),
        static_cast<uint32_t>(height - 1 - y),
        &pixel);
    }
  }

  geBitmap_UnLock(pLock);

  return fiBitmap;
}

Result<kdl::resource<FIBITMAP*>> createFreeImageFromGeBitmap24Bit(geBitmap* pBitmap)
{
  geBitmap* pLock;
  if (!geBitmap_LockForRead(pBitmap, &pLock, 0, 0, GE_PIXELFORMAT_24BIT_BGR, false, 0))
  {
    return Error{"Error locking geBitmap for reading."};
  }

  geBitmap_Info bitmapInfo;
  geBitmap_GetInfo(pLock, &bitmapInfo, nullptr);
  auto width = bitmapInfo.Width;
  auto height = bitmapInfo.Height;

  // Create a FreeImage bitmap of 32 bits to simplify the conversion
  InitFreeImage::initialize();
  auto fiBitmap = kdl::resource{FreeImage_Allocate(width, height, 32), FreeImage_Unload};
  if (!fiBitmap)
  {
    geBitmap_UnLock(pLock);
    return Error{"Failed to create bitmap"};
  }

  bool hasAlpha = geBitmap_HasAlpha(pBitmap);
  geBitmap* pLockAlpha = nullptr;

  if (
    hasAlpha
    && !geBitmap_LockForRead(
      geBitmap_GetAlpha(pBitmap), &pLockAlpha, 0, 0, GE_PIXELFORMAT_8BIT_GRAY, false, 0))
  {
    geBitmap_UnLock(pLock);
    return Error{"Error locking geBitmap alpha for reading."};
  }

  auto* pPixelData = (BYTE*)geBitmap_GetBits(pLock);
  auto* pPixelDataAlpha = hasAlpha ? (BYTE*)geBitmap_GetBits(pLockAlpha) : nullptr;

  // Copy pixel data to FreeImage bitmap
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      int pixelIndex = y * width * 3 + x * 3; // 24-bit image
      RGBQUAD pixel;
      pixel.rgbReserved = hasAlpha ? pPixelDataAlpha[(y * width + x)] : (BYTE)255;
      pixel.rgbBlue = pPixelData[pixelIndex + 0];
      pixel.rgbGreen = pPixelData[pixelIndex + 1];
      pixel.rgbRed = pPixelData[pixelIndex + 2];

      // Set the pixel color in the FreeImage bitmap
      FreeImage_SetPixelColor(
        *fiBitmap,
        static_cast<uint32_t>(x),
        static_cast<uint32_t>(height - 1 - y),
        &pixel);
    }
  }

  geBitmap_UnLock(pLock); // Unlock the main bitmap
  if (pLockAlpha)
    geBitmap_UnLock(pLockAlpha); // Unlock the alpha bitmap if it was locked

  return fiBitmap;
}
} // namespace

Result<mdl::Texture> readGeBitmapTexture(Reader& reader)
{
  auto bufferedReader = reader.buffer();
  const auto* begin = bufferedReader.begin();
  const auto* end = bufferedReader.end();
  const auto imageSize = size_t(end - begin);

  if (begin == end)
  {
    return Error{"No data in buffered reader"};
  }
  auto imageData = std::vector<char>(imageSize);
  bufferedReader.read(imageData.data(), imageSize);

  geVFile_MemoryContext context{imageData.data(), static_cast<int32_t>(imageSize)};
  auto* memFile = geVFile_OpenNewSystem(
    nullptr, GE_VFILE_TYPE_MEMORY, nullptr, &context, GE_VFILE_OPEN_READONLY);

  if (!memFile)
  {
    return Error{"Failed to load geBitmap file in memory"};
  }

  geBitmap* bitmap = geBitmap_CreateFromFile(memFile);
  geVFile_Close(memFile);

  if (!bitmap)
  {
    return Error{"File is not a valid bitmap"};
  }

  auto image = createTextureFromGeBitmap(bitmap);

  if (!image.is_success())
  {
    geBitmap_Destroy(&bitmap);
    return Error{"Failed to create FreeImage bitmap from a geBitmap"};
  }

  geBitmap_Destroy(&bitmap);

  return image;
}

Result<mdl::Texture> createTextureFromGeBitmap(geBitmap* bitmap)
{
  geBitmap_Info bitmapInfo;
  geBitmap_GetInfo(bitmap, &bitmapInfo, nullptr);
  auto image = bitmapInfo.Format == GE_PIXELFORMAT_8BIT
                 ? createFreeImageFromGeBitmap8Bit(bitmap)
                 : createFreeImageFromGeBitmap24Bit(bitmap);
  if (!image.is_success())
  {
    return Error{"Failed to create FreeImage bitmap from a geBitmap"};
  }
  return createTextureFromFreeImageBitmap(*image.value());
}

} // namespace tb::io
