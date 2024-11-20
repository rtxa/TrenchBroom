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

#include "TxlFileSystem.h"

#include "io/File.h"
#include "io/Reader.h"
#include "io/ReaderException.h"

#include "kdl/result.h"
#include "kdl/string_format.h"
#include "kdl/string_utils.h"

#include <fmt/format.h>

namespace tb::io
{
namespace geVfs
{
struct VFSFileHeader
{
  uint32_t Signature;
  uint16_t Version;        // Version number
  int32_t Dispersed;       // Is this VFS dispersed?
  int32_t DirectoryOffset; // File offset to directory
  uint32_t DataLength;     // Length of all file data, including VFS header
  uint32_t EndPosition;    // End Position in the RWOps file we were written to
};

struct DirTreeHeader
{
  uint32_t Signature;
  int32_t Size;
};

enum geVFile_Attributes
{
  None = 0,
  ReadOnly = (1 << 0),
  Directory = (2 << 0)
};

struct geVFile_Time
{
  uint32_t Time1;
  uint32_t Time2;
};

struct geVFile_Hints
{
  uint32_t HintDataLength;
  std::string HintData;
};

struct DirTree
{
  std::string Name;
  geVFile_Time Time;
  uint32_t AttributeFlags;
  uint32_t Size;
  uint32_t Offset;
  geVFile_Hints Hints;
  std::unique_ptr<DirTree> Parent;
  std::unique_ptr<DirTree> Children;
  std::unique_ptr<DirTree> Siblings;
};

#define DIRTREE_LIST_TERMINATED 0xFFFFFFFF

struct FileEntry
{
  std::string Name;
  uint32_t Offset;
  uint32_t Size;
};

std::vector<FileEntry> GetFileEntries(const DirTree* tree)
{
  std::vector<FileEntry> fileEntries;
  if (tree)
  {
    if (!(tree->AttributeFlags & Directory))
    {
      FileEntry entry;
      entry.Name = tree->Name;
      entry.Offset = tree->Offset;
      entry.Size = tree->Size;
      fileEntries.push_back(entry);
    }
    if (tree->Children)
    {
      auto childEntries = GetFileEntries(tree->Children.get());
      fileEntries.insert(fileEntries.end(), childEntries.begin(), childEntries.end());
    }
    if (tree->Siblings)
    {
      auto siblingEntries = GetFileEntries(tree->Siblings.get());
      fileEntries.insert(fileEntries.end(), siblingEntries.begin(), siblingEntries.end());
    }
  }
  return fileEntries;
}

template <typename T>
bool ReadFromFile(tb::io::Reader& file, T& data)
{
  file.read(reinterpret_cast<char*>(&data), sizeof(T));
  return true;
}

bool ReadStringFromFile(tb::io::Reader& file, std::string& outString, int32_t length)
{
  if (length <= 0)
  {
    return false;
  }

  outString.resize(static_cast<uint32_t>(length));
  file.read(&outString[0], static_cast<uint32_t>(length));
  return true;
}

// ReadTree function
bool ReadTree(tb::io::Reader& file, std::unique_ptr<DirTree>& TreePtr)
{
  // Read the terminator value
  int32_t terminator;
  file.read(reinterpret_cast<char*>(&terminator), sizeof(int32_t));

  if (terminator == DIRTREE_LIST_TERMINATED)
  {
    TreePtr.reset(); // Set TreePtr to nullptr
    return true;
  }

  // Allocate space for the DirTree
  TreePtr = std::make_unique<DirTree>();
  if (!TreePtr)
  {
    return false;
  }

  // Read the name length
  int32_t nameLength;
  file.read(reinterpret_cast<char*>(&nameLength), sizeof(int32_t));

  ReadStringFromFile(file, TreePtr->Name, nameLength);
  TreePtr->Name.pop_back(); // Remove the null terminator

  // Read out the attribute information
  if (
    !ReadFromFile(file, TreePtr->Time) || !ReadFromFile(file, TreePtr->AttributeFlags)
    || !ReadFromFile(file, TreePtr->Size) || !ReadFromFile(file, TreePtr->Offset)
    || !ReadFromFile(file, TreePtr->Hints.HintDataLength))
  {
    return false;
  }

  int32_t hintLength = static_cast<int32_t>(TreePtr->Hints.HintDataLength);
  ReadStringFromFile(file, TreePtr->Hints.HintData, hintLength);

  // Read the children
  if (!ReadTree(file, TreePtr->Children))
  {
    return false;
  }

  // Read the siblings
  if (!ReadTree(file, TreePtr->Siblings))
  {
    return false;
  }

  return true;
}
} // namespace geVfs

TxlFileSystem::TxlFileSystem(std::shared_ptr<CFile> file)
  : ImageFileSystem{file->buffer()}
{
}


Result<void> TxlFileSystem::doReadDirectory()
{
  auto reader = m_file->reader();
  geVfs::VFSFileHeader header;
  geVfs::DirTreeHeader dirHeader;
  std::unique_ptr<geVfs::DirTree> root;

  // Get directory offset from where we are going to read the directory tree
  reader.read(reinterpret_cast<char*>(&header), sizeof(geVfs::VFSFileHeader));
  if (header.DirectoryOffset < 0)
    return Error{"Invalid directory offset"};
  reader.seekFromBegin(static_cast<uint32_t>(header.DirectoryOffset));

  reader.read(reinterpret_cast<char*>(&dirHeader), sizeof(geVfs::DirTreeHeader));
  ReadTree(reader, root);

  auto entries = GetFileEntries(root.get());
  for (const auto& entry : entries)
  {
    const auto path = std::filesystem::path{entry.Name + ".gebmp"};
    auto file_ = std::static_pointer_cast<File>(
      std::make_shared<FileView>(m_file, entry.Offset, entry.Size));
    addFile(path, [file = std::move(file_)]() -> Result<std::shared_ptr<File>> {
      return file;
    });
  }
  return kdl::void_success;
}
} // namespace tb::io
