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

#include "View/SmartPropertyEditor.h"

#include <memory>
#include <vector>

class QComboBox;
class QWidget;
class QLabel;
class QWidget;

namespace tb::Assets
{
class ChoicePropertyDefinition;
}

namespace tb::View
{
class MapDocument;

class SmartChoiceEditor : public SmartPropertyEditor
{
  Q_OBJECT
private:
  QComboBox* m_comboBox = nullptr;
  bool m_ignoreEditTextChanged = false;

public:
  explicit SmartChoiceEditor(
    std::weak_ptr<MapDocument> document, QWidget* parent = nullptr);

  void comboBoxActivated(int index);
  void comboBoxEditTextChanged(const QString& text);

private:
  void createGui();
  void doUpdateVisual(const std::vector<Model::EntityNodeBase*>& nodes) override;
};

} // namespace tb::View
