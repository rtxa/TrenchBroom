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

#pragma once

#include "Assets/Material.h"
#include "Assets/Palette.h"

#include <filesystem>
#include <optional>

namespace tb
{
class Logger;
} // namespace tb

namespace tb::IO
{
class FileSystem;

Assets::Material loadSkin(
  const std::filesystem::path& path, const FileSystem& fs, Logger& logger);

Assets::Material loadSkin(
  const std::filesystem::path& path,
  const FileSystem& fs,
  const std::optional<Assets::Palette>& palette,
  Logger& logger);

} // namespace tb::IO
