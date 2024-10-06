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

#include "EL/VariableStore.h"
#include "Macros.h"

#include <string>
#include <vector>

namespace tb::Model
{
class Entity;

class EntityPropertiesVariableStore : public EL::VariableStore
{
private:
  const Entity& m_entity;

public:
  explicit EntityPropertiesVariableStore(const Entity& entity);

  VariableStore* clone() const override;
  size_t size() const override;
  EL::Value value(const std::string& name) const override;
  std::vector<std::string> names() const override;
  void declare(const std::string& name, const EL::Value& value) override;
  void assign(const std::string& name, const EL::Value& value) override;

  deleteCopyAndMove(EntityPropertiesVariableStore);
};

} // namespace tb::Model
