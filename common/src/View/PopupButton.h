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

#include <QWidget>

class QToolButton;

namespace tb::View
{
class PopupWindow;

class PopupButton : public QWidget
{
  Q_OBJECT
private:
  QToolButton* m_button = nullptr;
  PopupWindow* m_window = nullptr;

public:
  explicit PopupButton(const QString& caption = "", QWidget* parent = nullptr);

  QWidget* GetPopupWindow() const;

private:
  void buttonClicked(bool checked);
  void popupVisibilityChanged(bool visible);
};

} // namespace tb::View
