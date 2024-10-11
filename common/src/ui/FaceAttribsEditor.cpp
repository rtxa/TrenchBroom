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

#include "FaceAttribsEditor.h"

#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QVBoxLayout>
#include <QtGlobal>

#include "Color.h"
#include "mdl/BrushFace.h"
#include "mdl/BrushFaceHandle.h"
#include "mdl/ChangeBrushFaceAttributesRequest.h"
#include "mdl/Game.h"
#include "mdl/GameConfig.h"
#include "mdl/MapFormat.h"
#include "mdl/Material.h"
#include "mdl/Texture.h"
#include "mdl/WorldNode.h"
#include "ui/BorderLine.h"
#include "ui/FlagsPopupEditor.h"
#include "ui/Grid.h"
#include "ui/MapDocument.h"
#include "ui/QtUtils.h"
#include "ui/SignalDelayer.h"
#include "ui/SpinControl.h"
#include "ui/UVEditor.h"
#include "ui/ViewConstants.h"
#include "ui/ViewUtils.h"

#include "kdl/memory_utils.h"
#include "kdl/string_format.h"
#include "kdl/string_utils.h"

#include "vm/vec_io.h" // IWYU pragma: keep

#include <memory>
#include <string>

namespace tb::ui
{

FaceAttribsEditor::FaceAttribsEditor(
  std::weak_ptr<MapDocument> document, GLContextManager& contextManager, QWidget* parent)
  : QWidget{parent}
  , m_document{std::move(document)}
  , m_updateControlsSignalDelayer{new SignalDelayer{this}}
{
  createGui(contextManager);
  bindEvents();
  connectObservers();
  updateIncrements();
}

bool FaceAttribsEditor::cancelMouseDrag()
{
  return m_uvEditor->cancelMouseDrag();
}

void FaceAttribsEditor::xOffsetChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setXOffset(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::yOffsetChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setYOffset(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::rotationChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setRotation(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::xScaleChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setXScale(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::yScaleChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setYScale(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::surfaceFlagChanged(
  const size_t /* index */, const int value, const int setFlag, const int /* mixedFlag */)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  if (setFlag & value)
  {
    request.setSurfaceFlags(value);
  }
  else
  {
    request.unsetSurfaceFlags(value);
  }
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::contentFlagChanged(
  const size_t /* index */, const int value, const int setFlag, const int /* mixedFlag */)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  if (setFlag & value)
  {
    request.setContentFlags(value);
  }
  else
  {
    request.unsetContentFlags(value);
  }
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::surfaceValueChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setSurfaceValue(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::colorValueChanged(const QString& /* text */)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  const std::string str = m_colorEditor->text().toStdString();
  if (!kdl::str_is_blank(str))
  {
    if (const auto color = Color::parse(str))
    {
      auto request = mdl::ChangeBrushFaceAttributesRequest{};
      request.setColor(*color);
      if (!document->setFaceAttributes(request))
      {
        updateControls();
      }
    }
  }
  else
  {
    auto request = mdl::ChangeBrushFaceAttributesRequest{};
    request.setColor(Color());
    if (!document->setFaceAttributes(request))
    {
      updateControls();
    }
  }
}

void FaceAttribsEditor::transparencyValueChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setTransparencyValue(int(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::reflectivityScaleChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setReflectivityScale(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::xLightMapScaleChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setXLightMapScale(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}
void FaceAttribsEditor::yLightMapScaleChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setYLightMapScale(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}


void FaceAttribsEditor::mipMapBiasChanged(const double value)
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setMipMapBias(float(value));
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::surfaceFlagsUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.replaceSurfaceFlags(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::contentFlagsUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.replaceContentFlags(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::surfaceValueUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setSurfaceValue(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::colorValueUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setColor(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::transparencyValueUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setTransparencyValue(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::reflectivityScaleUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setReflectivityScale(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::xLightMapScaleUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setXLightMapScale(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::yLightMapScaleUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setYLightMapScale(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::mipMapBiasUnset()
{
  auto document = kdl::mem_lock(m_document);
  if (!document->hasAnySelectedBrushFaces())
  {
    return;
  }

  auto request = mdl::ChangeBrushFaceAttributesRequest{};
  request.setMipMapBias(std::nullopt);
  if (!document->setFaceAttributes(request))
  {
    updateControls();
  }
}

void FaceAttribsEditor::updateIncrements()
{
  auto document = kdl::mem_lock(m_document);
  Grid& grid = document->grid();

  m_xOffsetEditor->setIncrements(grid.actualSize(), 2.0 * grid.actualSize(), 1.0);
  m_yOffsetEditor->setIncrements(grid.actualSize(), 2.0 * grid.actualSize(), 1.0);
  m_rotationEditor->setIncrements(vm::to_degrees(grid.angle()), 90.0, 1.0);
}

static QWidget* createUnsetButtonLayout(QWidget* expandWidget, QWidget* button)
{
  auto* wrapper = new QWidget();
  auto* rowLayout = new QHBoxLayout();
  rowLayout->setContentsMargins(0, 0, 0, 0);
  rowLayout->setSpacing(LayoutConstants::NarrowHMargin);
  rowLayout->addWidget(expandWidget, 1);
  rowLayout->addWidget(button);
  wrapper->setLayout(rowLayout);
  return wrapper;
}

void FaceAttribsEditor::createGui(GLContextManager& contextManager)
{
  m_uvEditor = new UVEditor{m_document, contextManager};

  auto* materialNameLabel = new QLabel{"Material"};
  makeEmphasized(materialNameLabel);
  m_materialName = new QLabel{"none"};
  m_materialName->setTextInteractionFlags(Qt::TextSelectableByMouse);

  auto* textureSizeLabel = new QLabel{"Size"};
  makeEmphasized(textureSizeLabel);
  m_textureSize = new QLabel{""};

  const auto max = std::numeric_limits<double>::max();
  const auto min = -max;

  auto* xOffsetLabel = new QLabel{"X Offset"};
  makeEmphasized(xOffsetLabel);
  m_xOffsetEditor = new SpinControl{};
  m_xOffsetEditor->setRange(min, max);
  m_xOffsetEditor->setDigits(0, 6);

  auto* yOffsetLabel = new QLabel{"Y Offset"};
  makeEmphasized(yOffsetLabel);
  m_yOffsetEditor = new SpinControl{};
  m_yOffsetEditor->setRange(min, max);
  m_yOffsetEditor->setDigits(0, 6);

  auto* xScaleLabel = new QLabel{"X Scale"};
  makeEmphasized(xScaleLabel);
  m_xScaleEditor = new SpinControl{};
  m_xScaleEditor->setRange(min, max);
  m_xScaleEditor->setIncrements(0.1, 0.25, 0.01);
  m_xScaleEditor->setDigits(0, 6);

  auto* yScaleLabel = new QLabel{"Y Scale"};
  makeEmphasized(yScaleLabel);
  m_yScaleEditor = new SpinControl{};
  m_yScaleEditor->setRange(min, max);
  m_yScaleEditor->setIncrements(0.1, 0.25, 0.01);
  m_yScaleEditor->setDigits(0, 6);

  auto* rotationLabel = new QLabel{"Angle"};
  makeEmphasized(rotationLabel);
  m_rotationEditor = new SpinControl{};
  m_rotationEditor->setRange(min, max);
  m_rotationEditor->setDigits(0, 6);

  m_surfaceValueLabel = new QLabel{"Value"};
  makeEmphasized(m_surfaceValueLabel);
  m_surfaceValueEditor = new SpinControl{};
  m_surfaceValueEditor->setRange(min, max);
  m_surfaceValueEditor->setIncrements(1.0, 10.0, 100.0);
  m_surfaceValueEditor->setDigits(0, 6);
  m_surfaceValueUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset surface value"));
  m_surfaceValueEditorLayout =
    createUnsetButtonLayout(m_surfaceValueEditor, m_surfaceValueUnsetButton);

  m_surfaceFlagsLabel = new QLabel{"Surface"};
  makeEmphasized(m_surfaceFlagsLabel);
  m_surfaceFlagsEditor = new FlagsPopupEditor{2};
  m_surfaceFlagsUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset surface flags"));
  m_surfaceFlagsEditorLayout =
    createUnsetButtonLayout(m_surfaceFlagsEditor, m_surfaceFlagsUnsetButton);

  m_contentFlagsLabel = new QLabel{"Content"};
  makeEmphasized(m_contentFlagsLabel);
  m_contentFlagsEditor = new FlagsPopupEditor{2};
  m_contentFlagsUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset content flags"));
  m_contentFlagsEditorLayout =
    createUnsetButtonLayout(m_contentFlagsEditor, m_contentFlagsUnsetButton);

  m_colorLabel = new QLabel{"Color"};
  makeEmphasized(m_colorLabel);
  m_colorEditor = new QLineEdit{};
  m_colorUnsetButton = createBitmapButton("ResetUV.svg", tr("Unset color"));
  m_colorEditorLayout = createUnsetButtonLayout(m_colorEditor, m_colorUnsetButton);

  m_transparencyValueLabel = new QLabel{"Transparency"};
  makeEmphasized(m_transparencyValueLabel);
  m_transparencyValueEditor = new SpinControl{};
  m_transparencyValueEditor->setRange(0, 255);
  m_transparencyValueEditor->setIncrements(1.0, 10.0, 100.0);
  m_transparencyValueEditor->setDigits(0, 6);
  m_transparencyValueUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset transparency value"));
  m_transparencyValueEditorLayout =
    createUnsetButtonLayout(m_transparencyValueEditor, m_transparencyValueUnsetButton);

  m_reflectivityScaleLabel = new QLabel{"Reflectivity"};
  makeEmphasized(m_reflectivityScaleLabel);
  m_reflectivityScaleEditor = new SpinControl{};
  m_reflectivityScaleEditor->setRange(min, max);
  m_reflectivityScaleEditor->setIncrements(0.1, 0.25, 0.5);
  m_reflectivityScaleEditor->setDigits(0, 6);
  m_reflectivityScaleUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset reflectivity scale"));
  m_reflectivityScaleEditorLayout =
    createUnsetButtonLayout(m_reflectivityScaleEditor, m_reflectivityScaleUnsetButton);

  m_xLightMapScaleLabel = new QLabel{"X Light Map"};
  makeEmphasized(m_xLightMapScaleLabel);
  m_xLightMapScaleEditor = new SpinControl{};
  m_xLightMapScaleEditor->setRange(min, max);
  m_xLightMapScaleEditor->setIncrements(0.1, 0.25, 0.5);
  m_xLightMapScaleEditor->setDigits(0, 6);
  m_xLightMapScaleUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset X Light Map scale"));
  m_xLightMapScaleEditorLayout =
    createUnsetButtonLayout(m_xLightMapScaleEditor, m_xLightMapScaleUnsetButton);

  m_yLightMapScaleLabel = new QLabel{"Y Light Map"};
  makeEmphasized(m_yLightMapScaleLabel);
  m_yLightMapScaleEditor = new SpinControl{};
  m_yLightMapScaleEditor->setRange(min, max);
  m_yLightMapScaleEditor->setIncrements(0.1, 0.25, 0.5);
  m_yLightMapScaleEditor->setDigits(0, 6);
  m_yLightMapScaleUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset Y Light Map scale"));
  m_yLightMapScaleEditorLayout =
    createUnsetButtonLayout(m_yLightMapScaleEditor, m_yLightMapScaleUnsetButton);

  m_mipMapBiasLabel = new QLabel{"MipMap Bias"};
  makeEmphasized(m_mipMapBiasLabel);
  m_mipMapBiasEditor = new SpinControl{};
  m_mipMapBiasEditor->setRange(min, max);
  m_mipMapBiasEditor->setIncrements(0.1, 0.25, 0.5);
  m_mipMapBiasEditor->setDigits(0, 6);
  m_mipMapBiasUnsetButton =
    createBitmapButton("ResetUV.svg", tr("Unset mipmap bias scale"));
  m_mipMapBiasEditorLayout =
    createUnsetButtonLayout(m_mipMapBiasEditor, m_mipMapBiasUnsetButton);

  const Qt::Alignment LabelFlags = Qt::AlignVCenter | Qt::AlignRight;
  const Qt::Alignment ValueFlags = Qt::AlignVCenter;

  auto* faceAttribsLayout = new QGridLayout{};
  faceAttribsLayout->setContentsMargins(
    LayoutConstants::NarrowHMargin,
    LayoutConstants::MediumVMargin,
    LayoutConstants::NarrowHMargin,
    LayoutConstants::MediumVMargin);
  faceAttribsLayout->setHorizontalSpacing(LayoutConstants::MediumHMargin);
  faceAttribsLayout->setVerticalSpacing(LayoutConstants::MediumVMargin);

  int r = 0;
  int c = 0;

  faceAttribsLayout->addWidget(materialNameLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_materialName, r, c++, ValueFlags);
  faceAttribsLayout->addWidget(textureSizeLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_textureSize, r, c++, ValueFlags);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(xOffsetLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_xOffsetEditor, r, c++);
  faceAttribsLayout->addWidget(yOffsetLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_yOffsetEditor, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(xScaleLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_xScaleEditor, r, c++);
  faceAttribsLayout->addWidget(yScaleLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_yScaleEditor, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(rotationLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_rotationEditor, r, c++);
  faceAttribsLayout->addWidget(m_surfaceValueLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_surfaceValueEditorLayout, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_surfaceFlagsLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_surfaceFlagsEditorLayout, r, c++, 1, 3);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_contentFlagsLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_contentFlagsEditorLayout, r, c++, 1, 3);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_colorLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_colorEditorLayout, r, c++, 1, 3);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_xLightMapScaleLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_xLightMapScaleEditorLayout, r, c++);
  faceAttribsLayout->addWidget(m_yLightMapScaleLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_yLightMapScaleEditorLayout, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_transparencyValueLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_transparencyValueEditorLayout, r, c++);
  faceAttribsLayout->addWidget(m_reflectivityScaleLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_reflectivityScaleEditorLayout, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->addWidget(m_mipMapBiasLabel, r, c++, LabelFlags);
  faceAttribsLayout->addWidget(m_mipMapBiasEditorLayout, r, c++);
  ++r;
  c = 0;

  faceAttribsLayout->setColumnStretch(1, 1);
  faceAttribsLayout->setColumnStretch(3, 1);

  auto* outerLayout = new QVBoxLayout{};
  outerLayout->setContentsMargins(0, 0, 0, 0);
  outerLayout->setSpacing(LayoutConstants::NarrowVMargin);
  outerLayout->addWidget(m_uvEditor, 1);
  outerLayout->addWidget(new BorderLine{});
  outerLayout->addLayout(faceAttribsLayout);

  setLayout(outerLayout);
}

void FaceAttribsEditor::bindEvents()
{
  connect(
    m_xOffsetEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::xOffsetChanged);
  connect(
    m_yOffsetEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::yOffsetChanged);
  connect(
    m_xScaleEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::xScaleChanged);
  connect(
    m_yScaleEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::yScaleChanged);
  connect(
    m_rotationEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::rotationChanged);
  connect(
    m_surfaceValueEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::surfaceValueChanged);
  connect(
    m_surfaceFlagsEditor,
    &FlagsPopupEditor::flagChanged,
    this,
    &FaceAttribsEditor::surfaceFlagChanged);
  connect(
    m_contentFlagsEditor,
    &FlagsPopupEditor::flagChanged,
    this,
    &FaceAttribsEditor::contentFlagChanged);
  connect(
    m_colorEditor, &QLineEdit::textEdited, this, &FaceAttribsEditor::colorValueChanged);
  connect(
    m_transparencyValueEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::transparencyValueChanged);
  connect(
    m_reflectivityScaleEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::reflectivityScaleChanged);
  connect(
    m_xLightMapScaleEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::xLightMapScaleChanged);
  connect(
    m_yLightMapScaleEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::yLightMapScaleChanged);
  connect(
    m_mipMapBiasEditor,
    QOverload<double>::of(&QDoubleSpinBox::valueChanged),
    this,
    &FaceAttribsEditor::mipMapBiasChanged);
  connect(
    m_surfaceValueUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::surfaceValueUnset);
  connect(
    m_surfaceFlagsUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::surfaceFlagsUnset);
  connect(
    m_contentFlagsUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::contentFlagsUnset);
  connect(
    m_colorUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::colorValueUnset);
  connect(
    m_transparencyValueUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::transparencyValueUnset);
  connect(
    m_reflectivityScaleUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::reflectivityScaleUnset);
  connect(
    m_xLightMapScaleUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::xLightMapScaleUnset);
  connect(
    m_yLightMapScaleUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::yLightMapScaleUnset);
  connect(
    m_mipMapBiasUnsetButton,
    &QAbstractButton::clicked,
    this,
    &FaceAttribsEditor::mipMapBiasUnset);
  connect(
    m_updateControlsSignalDelayer,
    &SignalDelayer::processSignal,
    this,
    &FaceAttribsEditor::updateControls);
}

void FaceAttribsEditor::connectObservers()
{
  auto document = kdl::mem_lock(m_document);
  m_notifierConnection += document->documentWasNewedNotifier.connect(
    this, &FaceAttribsEditor::documentWasNewed);
  m_notifierConnection += document->documentWasLoadedNotifier.connect(
    this, &FaceAttribsEditor::documentWasLoaded);
  m_notifierConnection +=
    document->nodesDidChangeNotifier.connect(this, &FaceAttribsEditor::nodesDidChange);
  m_notifierConnection += document->brushFacesDidChangeNotifier.connect(
    this, &FaceAttribsEditor::brushFacesDidChange);
  m_notifierConnection += document->selectionDidChangeNotifier.connect(
    this, &FaceAttribsEditor::selectionDidChange);
  m_notifierConnection += document->materialCollectionsDidChangeNotifier.connect(
    this, &FaceAttribsEditor::materialCollectionsDidChange);
  m_notifierConnection += document->grid().gridDidChangeNotifier.connect(
    this, &FaceAttribsEditor::updateIncrements);
}

void FaceAttribsEditor::documentWasNewed(MapDocument*)
{
  updateControls();
}

void FaceAttribsEditor::documentWasLoaded(MapDocument*)
{
  updateControls();
}

void FaceAttribsEditor::nodesDidChange(const std::vector<mdl::Node*>&)
{
  updateControlsDelayed();
}

void FaceAttribsEditor::brushFacesDidChange(const std::vector<mdl::BrushFaceHandle>&)
{
  updateControlsDelayed();
}

void FaceAttribsEditor::selectionDidChange(const Selection&)
{
  updateControlsDelayed();
}

void FaceAttribsEditor::materialCollectionsDidChange()
{
  updateControls();
}

static void disableAndSetPlaceholder(QDoubleSpinBox* box, const QString& text)
{
  box->setSpecialValueText(text);
  box->setValue(box->minimum());
  box->setEnabled(false);
}

static void setValueOrMulti(QDoubleSpinBox* box, const bool multi, const double value)
{
  if (multi)
  {
    box->setSpecialValueText("multi");
    box->setValue(box->minimum());
  }
  else
  {
    box->setSpecialValueText("");
    box->setValue(value);
  }
}

void FaceAttribsEditor::updateControls()
{
  // block signals emitted when updating the editor values
  const auto blockXOffsetEditor = QSignalBlocker{m_xOffsetEditor};
  const auto blockYOffsetEditor = QSignalBlocker{m_yOffsetEditor};
  const auto blockRotationEditor = QSignalBlocker{m_rotationEditor};
  const auto blockXScaleEditor = QSignalBlocker{m_xScaleEditor};
  const auto blockYScaleEditor = QSignalBlocker{m_yScaleEditor};
  const auto blockSurfaceValueEditor = QSignalBlocker{m_surfaceValueEditor};
  const auto blockSurfaceFlagsEditor = QSignalBlocker{m_surfaceFlagsEditor};
  const auto blockContentFlagsEditor = QSignalBlocker{m_contentFlagsEditor};
  const auto blockColorEditor = QSignalBlocker{m_colorEditor};
  const auto blockTransparencyEditor = QSignalBlocker{m_transparencyValueEditor};
  const auto blockReflectivityScaleEditor = QSignalBlocker{m_reflectivityScaleEditor};
  const auto blockXLightMapScaleEditor = QSignalBlocker{m_xLightMapScaleEditor};
  const auto blockYLightMapScaleEditor = QSignalBlocker{m_yLightMapScaleEditor};
  const auto blockMipMapBiasEditor = QSignalBlocker{m_mipMapBiasEditor};

  if (hasSurfaceFlags())
  {
    showSurfaceFlagsEditor();
    const auto [values, labels, tooltips] = getSurfaceFlags();
    m_surfaceFlagsEditor->setFlags(values, labels, tooltips);
  }
  else
  {
    hideSurfaceFlagsEditor();
  }

  if (hasContentFlags())
  {
    showContentFlagsEditor();
    const auto [values, labels, tooltips] = getContentFlags();
    m_contentFlagsEditor->setFlags(values, labels, tooltips);
  }
  else
  {
    hideContentFlagsEditor();
  }

  if (hasColorAttribs())
  {
    showColorAttribEditor();
  }
  else
  {
    hideColorAttribEditor();
  }

  if (hasGenesisAttribs())
  {
    showGenesisAttribsEditor();
  }
  else
  {
    hideGenesisAttribsEditor();
  }

  const auto faceHandles = kdl::mem_lock(m_document)->allSelectedBrushFaces();
  if (!faceHandles.empty())
  {
    auto materialMulti = false;
    auto xOffsetMulti = false;
    auto yOffsetMulti = false;
    auto rotationMulti = false;
    auto xScaleMulti = false;
    auto yScaleMulti = false;
    auto surfaceValueMulti = false;
    auto colorValueMulti = false;
    auto transparencyValueMulti = false;
    auto reflectivityScaleMulti = false;
    auto xLightMapScaleMulti = false;
    auto yLightMapScaleMulti = false;
    auto mipMapBiasMulti = false;

    const auto& firstFace = faceHandles[0].face();
    const auto& materialName = firstFace.attributes().materialName();
    const auto xOffset = firstFace.attributes().xOffset();
    const auto yOffset = firstFace.attributes().yOffset();
    const auto rotation = firstFace.attributes().rotation();
    const auto xScale = firstFace.attributes().xScale();
    const auto yScale = firstFace.attributes().yScale();
    auto setSurfaceFlags = firstFace.resolvedSurfaceFlags();
    auto setSurfaceContents = firstFace.resolvedSurfaceContents();
    auto mixedSurfaceFlags = 0;
    auto mixedSurfaceContents = 0;
    const auto surfaceValue = firstFace.resolvedSurfaceValue();
    const auto colorValue = firstFace.attributes().color();
    const auto transparencyValue = firstFace.resolvedTransparencyValue();
    const auto reflectivityScale = firstFace.resolvedReflectivityScale();
    const auto xLightMapScale = firstFace.resolvedXLightMapScale();
    const auto yLightMapScale = firstFace.resolvedYLightMapScale();
    const auto mipMapBias = firstFace.resolvedMipMapBias();
    auto hasSurfaceValue = firstFace.attributes().surfaceValue().has_value();
    auto hasSurfaceFlags = firstFace.attributes().surfaceFlags().has_value();
    auto hasSurfaceContents = firstFace.attributes().surfaceContents().has_value();
    auto hasColorValue = firstFace.attributes().hasColor();
    auto hasTransparencyValue =
      firstFace.attributes().transparencyValue().value_or(255) != 255;
    auto hasReflectivityScale =
      firstFace.attributes().reflectivityScale().value_or(1.0) != 1.0f;
    auto hasXLightMapScale =
      firstFace.attributes().xLightMapScale().value_or(1.0) != 1.0f;
    auto hasYLightMapScale =
      firstFace.attributes().yLightMapScale().value_or(1.0) != 1.0f;
    auto hasMipMapBias = firstFace.attributes().mipMapBias().value_or(1.0) != 1.0f;

    for (size_t i = 1; i < faceHandles.size(); i++)
    {
      const auto& face = faceHandles[i].face();
      materialMulti |= (materialName != face.attributes().materialName());
      xOffsetMulti |= (xOffset != face.attributes().xOffset());
      yOffsetMulti |= (yOffset != face.attributes().yOffset());
      rotationMulti |= (rotation != face.attributes().rotation());
      xScaleMulti |= (xScale != face.attributes().xScale());
      yScaleMulti |= (yScale != face.attributes().yScale());
      surfaceValueMulti |= (surfaceValue != face.resolvedSurfaceValue());
      colorValueMulti |= (colorValue != face.attributes().color());
      transparencyValueMulti |= (transparencyValue != face.resolvedTransparencyValue());
      reflectivityScaleMulti |= (reflectivityScale != face.resolvedReflectivityScale());
      xLightMapScaleMulti |= (xLightMapScale != face.resolvedXLightMapScale());
      yLightMapScaleMulti |= (yLightMapScale != face.resolvedYLightMapScale());
      mipMapBiasMulti |= (mipMapBias != face.resolvedMipMapBias());
      hasSurfaceValue |= face.attributes().surfaceValue().has_value();
      hasSurfaceFlags |= face.attributes().surfaceFlags().has_value();
      hasSurfaceContents |= face.attributes().surfaceContents().has_value();
      hasColorValue |= face.attributes().hasColor();
      hasTransparencyValue = face.attributes().transparencyValue().value_or(255) != 255;
      hasReflectivityScale = face.attributes().reflectivityScale().value_or(1.0) != 1.0f;
      hasXLightMapScale = face.attributes().xLightMapScale().value_or(1.0) != 1.0f;
      hasYLightMapScale = face.attributes().yLightMapScale().value_or(1.0) != 1.0f;
      hasMipMapBias = face.attributes().mipMapBias().value_or(1.0) != 1.0f;

      combineFlags(
        sizeof(int) * 8, face.resolvedSurfaceFlags(), setSurfaceFlags, mixedSurfaceFlags);
      combineFlags(
        sizeof(int) * 8,
        face.resolvedSurfaceContents(),
        setSurfaceContents,
        mixedSurfaceContents);
    }

    m_xOffsetEditor->setEnabled(true);
    m_yOffsetEditor->setEnabled(true);
    m_rotationEditor->setEnabled(true);
    m_xScaleEditor->setEnabled(true);
    m_yScaleEditor->setEnabled(true);
    m_surfaceValueEditor->setEnabled(true);
    m_surfaceFlagsEditor->setEnabled(true);
    m_contentFlagsEditor->setEnabled(true);
    m_colorEditor->setEnabled(true);
    m_transparencyValueEditor->setEnabled(true);
    m_reflectivityScaleEditor->setEnabled(true);
    m_xLightMapScaleEditor->setEnabled(true);
    m_yLightMapScaleEditor->setEnabled(true);
    m_mipMapBiasEditor->setEnabled(true);

    if (materialMulti)
    {
      m_materialName->setText("multi");
      m_materialName->setEnabled(false);
      m_textureSize->setText("multi");
      m_textureSize->setEnabled(false);
    }
    else
    {
      if (materialName == mdl::BrushFaceAttributes::NoMaterialName)
      {
        m_materialName->setText("none");
        m_materialName->setEnabled(false);
        m_textureSize->setText("");
        m_textureSize->setEnabled(false);
      }
      else
      {
        if (const auto* texture = getTexture(firstFace.material()))
        {
          m_materialName->setText(QString::fromStdString(materialName));
          m_textureSize->setText(
            QStringLiteral("%1 * %2").arg(texture->width()).arg(texture->height()));
          m_materialName->setEnabled(true);
          m_textureSize->setEnabled(true);
        }
        else
        {
          m_materialName->setText(QString::fromStdString(materialName) + " (not found)");
          m_materialName->setEnabled(false);
          m_textureSize->setEnabled(false);
        }
      }
    }
    setValueOrMulti(m_xOffsetEditor, xOffsetMulti, double(xOffset));
    setValueOrMulti(m_yOffsetEditor, yOffsetMulti, double(yOffset));
    setValueOrMulti(m_rotationEditor, rotationMulti, double(rotation));
    setValueOrMulti(m_xScaleEditor, xScaleMulti, double(xScale));
    setValueOrMulti(m_yScaleEditor, yScaleMulti, double(yScale));
    setValueOrMulti(m_surfaceValueEditor, surfaceValueMulti, double(surfaceValue));
    setValueOrMulti(
      m_transparencyValueEditor, transparencyValueMulti, double(transparencyValue));
    setValueOrMulti(
      m_reflectivityScaleEditor, reflectivityScaleMulti, double(reflectivityScale));
    setValueOrMulti(m_xLightMapScaleEditor, xLightMapScaleMulti, double(xLightMapScale));
    setValueOrMulti(m_yLightMapScaleEditor, yLightMapScaleMulti, double(yLightMapScale));
    setValueOrMulti(m_mipMapBiasEditor, mipMapBiasMulti, double(mipMapBias));

    if (hasColorValue)
    {
      if (colorValueMulti)
      {
        m_colorEditor->setPlaceholderText("multi");
        m_colorEditor->setText("");
      }
      else
      {
        m_colorEditor->setPlaceholderText("");
        m_colorEditor->setText(QString::fromStdString(kdl::str_to_string(*colorValue)));
      }
    }
    else
    {
      m_colorEditor->setPlaceholderText("");
      m_colorEditor->setText("");
    }

    m_surfaceFlagsEditor->setFlagValue(setSurfaceFlags, mixedSurfaceFlags);
    m_contentFlagsEditor->setFlagValue(setSurfaceContents, mixedSurfaceContents);


    m_surfaceValueUnsetButton->setEnabled(hasSurfaceValue);
    m_surfaceFlagsUnsetButton->setEnabled(hasSurfaceFlags);
    m_contentFlagsUnsetButton->setEnabled(hasSurfaceContents);
    m_colorUnsetButton->setEnabled(hasColorValue);
    m_transparencyValueUnsetButton->setEnabled(hasTransparencyValue);
    m_reflectivityScaleUnsetButton->setEnabled(hasReflectivityScale);
    m_xLightMapScaleUnsetButton->setEnabled(hasXLightMapScale);
    m_yLightMapScaleUnsetButton->setEnabled(hasYLightMapScale);
    m_mipMapBiasUnsetButton->setEnabled(hasMipMapBias);
  }
  else
  {
    disableAndSetPlaceholder(m_xOffsetEditor, "n/a");
    disableAndSetPlaceholder(m_yOffsetEditor, "n/a");
    disableAndSetPlaceholder(m_xScaleEditor, "n/a");
    disableAndSetPlaceholder(m_yScaleEditor, "n/a");
    disableAndSetPlaceholder(m_rotationEditor, "n/a");
    disableAndSetPlaceholder(m_surfaceValueEditor, "n/a");
    disableAndSetPlaceholder(m_transparencyValueEditor, "n/a");
    disableAndSetPlaceholder(m_reflectivityScaleEditor, "n/a");
    disableAndSetPlaceholder(m_xLightMapScaleEditor, "n/a");
    disableAndSetPlaceholder(m_yLightMapScaleEditor, "n/a");
    disableAndSetPlaceholder(m_mipMapBiasEditor, "n/a");

    m_surfaceFlagsEditor->setEnabled(false);
    m_contentFlagsEditor->setEnabled(false);
    m_colorEditor->setText("");
    m_colorEditor->setPlaceholderText("n/a");
    m_colorEditor->setEnabled(false);

    m_transparencyValueEditor->setEnabled(false);
    m_reflectivityScaleEditor->setEnabled(false);
    m_xLightMapScaleEditor->setEnabled(false);
    m_yLightMapScaleEditor->setEnabled(false);
    m_mipMapBiasEditor->setEnabled(false);

    m_surfaceValueUnsetButton->setEnabled(false);
    m_surfaceFlagsUnsetButton->setEnabled(false);
    m_contentFlagsUnsetButton->setEnabled(false);
    m_colorUnsetButton->setEnabled(false);
    m_transparencyValueUnsetButton->setEnabled(false);
    m_reflectivityScaleUnsetButton->setEnabled(false);
    m_xLightMapScaleUnsetButton->setEnabled(false);
    m_yLightMapScaleUnsetButton->setEnabled(false);
    m_mipMapBiasUnsetButton->setEnabled(false);
  }
}

void FaceAttribsEditor::updateControlsDelayed()
{
  m_updateControlsSignalDelayer->queueSignal();
}

bool FaceAttribsEditor::hasSurfaceFlags() const
{
  auto document = kdl::mem_lock(m_document);
  const auto game = document->game();
  return !game->config().faceAttribsConfig.surfaceFlags.flags.empty();
}

bool FaceAttribsEditor::hasContentFlags() const
{
  auto document = kdl::mem_lock(m_document);
  const auto game = document->game();
  return !game->config().faceAttribsConfig.contentFlags.flags.empty();
}

void FaceAttribsEditor::showSurfaceFlagsEditor()
{
  m_surfaceValueLabel->show();
  m_surfaceValueEditorLayout->show();
  m_surfaceFlagsLabel->show();
  m_surfaceFlagsEditorLayout->show();
}

void FaceAttribsEditor::showContentFlagsEditor()
{
  m_contentFlagsLabel->show();
  m_contentFlagsEditorLayout->show();
}

void FaceAttribsEditor::hideSurfaceFlagsEditor()
{
  m_surfaceValueLabel->hide();
  m_surfaceValueEditorLayout->hide();
  m_surfaceFlagsLabel->hide();
  m_surfaceFlagsEditorLayout->hide();
}

void FaceAttribsEditor::hideContentFlagsEditor()
{
  m_contentFlagsLabel->hide();
  m_contentFlagsEditorLayout->hide();
}

bool FaceAttribsEditor::hasColorAttribs() const
{
  auto document = kdl::mem_lock(m_document);
  return document->world()->mapFormat() == mdl::MapFormat::Daikatana;
}

void FaceAttribsEditor::showColorAttribEditor()
{
  m_colorLabel->show();
  m_colorEditorLayout->show();
}

void FaceAttribsEditor::hideColorAttribEditor()
{
  m_colorLabel->hide();
  m_colorEditorLayout->hide();
}

bool FaceAttribsEditor::hasGenesisAttribs() const
{
  auto document = kdl::mem_lock(m_document);
  return document->world()->mapFormat() == mdl::MapFormat::Genesis3D;
}


void FaceAttribsEditor::showGenesisAttribsEditor()
{
  m_transparencyValueLabel->show();
  m_transparencyValueEditorLayout->show();
  m_reflectivityScaleLabel->show();
  m_reflectivityScaleEditorLayout->show();
  m_xLightMapScaleLabel->show();
  m_xLightMapScaleEditorLayout->show();
  m_yLightMapScaleLabel->show();
  m_yLightMapScaleEditorLayout->show();
  m_mipMapBiasLabel->show();
  m_mipMapBiasEditorLayout->show();
  m_surfaceValueLabel->setText(tr("Light Value"));
  m_surfaceValueUnsetButton->setToolTip(tr("Unset light value"));
}

void FaceAttribsEditor::hideGenesisAttribsEditor()
{
  m_transparencyValueLabel->hide();
  m_transparencyValueEditorLayout->hide();
  m_reflectivityScaleLabel->hide();
  m_reflectivityScaleEditorLayout->hide();
  m_xLightMapScaleLabel->hide();
  m_xLightMapScaleEditorLayout->hide();
  m_yLightMapScaleLabel->hide();
  m_yLightMapScaleEditorLayout->hide();
  m_mipMapBiasLabel->hide();
  m_mipMapBiasEditorLayout->hide();
  m_surfaceValueLabel->setText(tr("Value"));
  m_surfaceValueUnsetButton->setToolTip(tr("Unset value"));
}

namespace
{
std::tuple<QList<int>, QStringList, QStringList> getFlags(
  const std::vector<mdl::FlagConfig>& flags)
{
  auto values = QList<int>{};
  auto names = QStringList{};
  auto descriptions = QStringList{};

  for (const auto& flag : flags)
  {
    values.push_back(flag.value);
    names.push_back(QString::fromStdString(flag.name));
    descriptions.push_back(QString::fromStdString(flag.description));
  }

  return {std::move(values), std::move(names), std::move(descriptions)};
}
} // namespace

std::tuple<QList<int>, QStringList, QStringList> FaceAttribsEditor::getSurfaceFlags()
  const
{
  auto document = kdl::mem_lock(m_document);
  const auto game = document->game();
  const auto& surfaceFlags = game->config().faceAttribsConfig.surfaceFlags;
  return getFlags(surfaceFlags.flags);
}

std::tuple<QList<int>, QStringList, QStringList> FaceAttribsEditor::getContentFlags()
  const
{
  auto document = kdl::mem_lock(m_document);
  const auto game = document->game();
  const auto& contentFlags = game->config().faceAttribsConfig.contentFlags;
  return getFlags(contentFlags.flags);
}

} // namespace tb::ui
