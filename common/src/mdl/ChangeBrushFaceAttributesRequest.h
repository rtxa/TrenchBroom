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

#include "Color.h"

#include <optional>
#include <string>

namespace tb::mdl
{

class BrushFace;
class BrushFaceHandle;
class BrushFaceAttributes;

class ChangeBrushFaceAttributesRequest
{
public:
  enum class AxisOp
  {
    None,
    Reset,
    ToParaxial,
    ToParallel
  };

  enum class ValueOp
  {
    None,
    Set,
    Add,
    Mul
  };

  enum class FlagOp
  {
    None,
    Replace,
    Set,   // TODO: rename to SetBits
    Unset, // TODO: rename to UnsetBits or ClearBits
  };

  enum class MaterialOp
  {
    None,
    Set
  };

private:
  std::string m_materialName;
  float m_xOffset = 0.0f;
  float m_yOffset = 0.0f;
  float m_rotation = 0.0f;
  float m_xScale = 0.0f;
  float m_yScale = 0.0f;
  std::optional<int> m_surfaceFlags;
  std::optional<int> m_contentFlags;
  std::optional<float> m_surfaceValue;
  std::optional<Color> m_colorValue;
  std::optional<int> m_transparencyValue;
  std::optional<float> m_reflectivityScale;
  std::optional<float> m_xLightMapScale;
  std::optional<float> m_yLightMapScale;
  std::optional<float> m_mipMapBias;

  MaterialOp m_materialOp = MaterialOp::None;
  AxisOp m_axisOp = AxisOp::None;
  ValueOp m_xOffsetOp = ValueOp::None;
  ValueOp m_yOffsetOp = ValueOp::None;
  ValueOp m_rotationOp = ValueOp::None;
  ValueOp m_xScaleOp = ValueOp::None;
  ValueOp m_yScaleOp = ValueOp::None;
  FlagOp m_surfaceFlagsOp = FlagOp::None;
  FlagOp m_contentFlagsOp = FlagOp::None;
  ValueOp m_surfaceValueOp = ValueOp::None;
  ValueOp m_colorValueOp = ValueOp::None;
  ValueOp m_transparencyValueOp = ValueOp::None;
  ValueOp m_reflectivityScaleOp = ValueOp::None;
  ValueOp m_xLightMapScaleOp = ValueOp::None;
  ValueOp m_yLightMapScaleOp = ValueOp::None;
  ValueOp m_mipMapBiasOp = ValueOp::None;

public:
  ChangeBrushFaceAttributesRequest();

  void clear();

  const std::string name() const;
  bool evaluate(BrushFace& brushFace) const;

  void resetAll(const BrushFaceAttributes& defaultFaceAttributes);
  void resetAllToParaxial(const BrushFaceAttributes& defaultFaceAttributes);

  void setMaterialName(const std::string& materialName);

  void resetUVAxes();
  void resetUVAxesToParaxial();
  void resetUVAxesToParallel();

  void setOffset(const vm::vec2f& offset);
  void addOffset(const vm::vec2f& offset);
  void mulOffset(const vm::vec2f& offset);

  void setXOffset(float xOffset);
  void addXOffset(float xOffset);
  void mulXOffset(float xOffset);

  void setYOffset(float yOffset);
  void addYOffset(float yOffset);
  void mulYOffset(float yOffset);

  void setRotation(float rotation);
  void addRotation(float rotation);
  void mulRotation(float rotation);

  void setScale(const vm::vec2f& scale);
  void addScale(const vm::vec2f& scale);
  void mulScale(const vm::vec2f& scale);

  void setXScale(float xScale);
  void addXScale(float xScale);
  void mulXScale(float xScale);

  void setYScale(float yScale);
  void addYScale(float yScale);
  void mulYScale(float yScale);

  /**
   * When evaluated, the flags in `surfaceFlags` are set on the target face's surface
   * flags (leaving other surface flags on the target face as-is).
   */
  void setSurfaceFlags(int surfaceFlags);
  /**
   * When evaluated, the flags in `surfaceFlags` are cleared on the target face's surface
   * flags (leaving other surface flags on the target face as-is).
   */
  void unsetSurfaceFlags(int surfaceFlags);
  /**
   * When evaluated, replace the target face's surface flags with `surfaceFlags`.
   */
  void replaceSurfaceFlags(const std::optional<int>& surfaceFlags);

  void setContentFlags(int contentFlags);
  void unsetContentFlags(int contentFlags);
  void replaceContentFlags(const std::optional<int>& contentFlags);

  void setSurfaceValue(const std::optional<float>& surfaceValue);
  void addSurfaceValue(float surfaceValue);
  void mulSurfaceValue(float surfaceValue);

  void setColor(const std::optional<Color>& colorValue);
  void setTransparencyValue(const std::optional<int>& transparencyValue);

  void setReflectivityScale(const std::optional<float>& reflectivityScale);
  void addReflectivityScale(float reflectivityScale);
  void mulReflectivityScale(float reflectivityScale);

  void setXLightMapScale(const std::optional<float>& xLightMapScale);
  void addXLightMapScale(float xLightMapScale);
  void mulXLightMapScale(float xLightMapScale);

  void setYLightMapScale(const std::optional<float>& yLightMapScale);
  void addYLightMapScale(float yLightMapScale);
  void mulYLightMapScale(float yLightMapScale);

  void setMipMapBias(const std::optional<float>& mipMapBias);
  void addMipMapBias(float mipMapBias);
  void mulMipMapBias(float mipMapBias);

  /**
   * Configures `this` so, when evaluated, it transfers all attributes from the given
   * face to the evaluation target.
   */
  void setAll(const mdl::BrushFace& face);
  /**
   * Same as setAll(), but doesn't transfer content flags.
   */
  void setAllExceptContentFlags(const mdl::BrushFace& face);
  void setAll(const mdl::BrushFaceAttributes& attributes);
  void setAllExceptContentFlags(const mdl::BrushFaceAttributes& attributes);
};

} // namespace tb::mdl
