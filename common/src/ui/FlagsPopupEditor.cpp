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

#include "FlagsPopupEditor.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>

#include "ui/ElidedLabel.h"
#include "ui/FlagsEditor.h"
#include "ui/PopupButton.h"
#include "ui/ViewConstants.h"

namespace tb::ui
{

FlagsPopupEditor::FlagsPopupEditor(
  size_t numCols, const QString& buttonLabel, const bool showFlagsText, QWidget* parent)
  : QWidget{parent}
{
  m_button = new PopupButton{buttonLabel};
  m_button->setToolTip("Click to edit flags");

  auto* editorContainer = new QWidget{};
  m_editor = new FlagsEditor(numCols, editorContainer);

  auto* editorContainerLayout = new QVBoxLayout{};
  editorContainerLayout->setContentsMargins(0, 0, 0, 0);
  editorContainerLayout->setSpacing(0);
  editorContainerLayout->addWidget(m_editor);
  editorContainer->setLayout(editorContainerLayout);

  auto* popupLayout = new QVBoxLayout{};
  popupLayout->setContentsMargins(0, 0, 0, 0);
  popupLayout->setSpacing(0);
  popupLayout->addWidget(editorContainer);
  m_button->GetPopupWindow()->setLayout(popupLayout);

  auto* layout = new QHBoxLayout{};
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(LayoutConstants::MediumHMargin);

  if (showFlagsText)
  {
    m_flagsTxt = new ElidedLabel{Qt::ElideRight};

    auto flagsFrame = new QFrame{};
    flagsFrame->setFrameShape(QFrame::QFrame::StyledPanel);

    auto* flagsLayout = new QHBoxLayout{};
    flagsLayout->setContentsMargins(
      LayoutConstants::NarrowHMargin, 0, LayoutConstants::NarrowHMargin, 0);
    flagsLayout->setSpacing(0);
    flagsLayout->addWidget(m_flagsTxt);
    flagsFrame->setLayout(flagsLayout);

    layout->addWidget(flagsFrame, 1);
  }

  layout->addWidget(m_button, 0, Qt::AlignVCenter);
  setLayout(layout);

  connect(
    m_editor,
    &FlagsEditor::flagChanged,
    this,
    [this](
      const size_t /* index */,
      const int /* value */,
      const int /* setFlag */,
      const int /* mixedFlag */) { updateFlagsText(); });
  // forward this signal
  connect(m_editor, &FlagsEditor::flagChanged, this, &FlagsPopupEditor::flagChanged);
}

void FlagsPopupEditor::setDisabledFlags(int disabledFlags)
{
  m_editor->setDisabledFlags(disabledFlags);
}

void FlagsPopupEditor::setFlags(const QStringList& labels, const QStringList& tooltips)
{
  m_editor->setFlags(labels, tooltips);
  updateFlagsText();
}

void FlagsPopupEditor::setFlags(
  const QList<int>& values, const QStringList& labels, const QStringList& tooltips)
{
  m_editor->setFlags(values, labels, tooltips);
  updateFlagsText();
}

void FlagsPopupEditor::setFlagValue(const int set, const int mixed)
{
  m_editor->setFlagValue(set, mixed);
  updateFlagsText();
}

void FlagsPopupEditor::updateFlagsText()
{
  if (m_flagsTxt)
  {
    if (!isEnabled())
    {
      m_flagsTxt->setDisabled(true);
      m_flagsTxt->setText("n/a");
      m_flagsTxt->setToolTip("");
      return;
    }

    QString label;
    bool first = true;
    bool mixed = false;
    for (size_t i = 0; i < m_editor->getNumFlags() && !mixed; ++i)
    {
      if (m_editor->isFlagMixed(i))
      {
        label = "multi";
        mixed = true;
      }
      else if (m_editor->isFlagSet(i))
      {
        if (!first)
        {
          label += ", ";
        }
        label += m_editor->getFlagLabel(i);
        first = false;
      }
    }

    m_flagsTxt->setText(label);
    if (!first)
    {
      m_flagsTxt->setToolTip(label);
    }
    else
    {
      m_flagsTxt->setToolTip("");
    }

    m_flagsTxt->setDisabled(mixed);
  }
}

} // namespace tb::ui
