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

#include "PointHandleRenderer.h"

#include "PreferenceManager.h"
#include "Preferences.h"
#include "Renderer/ActiveShader.h"
#include "Renderer/Camera.h"
#include "Renderer/RenderContext.h"
#include "Renderer/Shaders.h"
#include "Renderer/VboManager.h"

#include "vm/mat_ext.h"
#include "vm/vec.h"

namespace tb::Renderer
{

PointHandleRenderer::PointHandleRenderer()
  : m_handle{pref(Preferences::HandleRadius), 16, true}
  , m_highlight{2.0f * pref(Preferences::HandleRadius), 16, false}
{
}

void PointHandleRenderer::addPoint(const Color& color, const vm::vec3f& position)
{
  m_pointHandles[color].push_back(position);
}

void PointHandleRenderer::addHighlight(const Color& color, const vm::vec3f& position)
{
  m_highlights[color].push_back(position);
}

void PointHandleRenderer::doPrepareVertices(VboManager& vboManager)
{
  m_handle.prepare(vboManager);
  m_highlight.prepare(vboManager);
}

void PointHandleRenderer::doRender(RenderContext& renderContext)
{
  const auto& camera = renderContext.camera();
  const auto& viewport = camera.viewport();
  const auto projection = vm::ortho_matrix(
    0.0f,
    1.0f,
    static_cast<float>(viewport.x),
    static_cast<float>(viewport.height),
    static_cast<float>(viewport.width),
    static_cast<float>(viewport.y));
  const auto view = vm::view_matrix(vm::vec3f{0, 0, -1}, vm::vec3f{0, 1, 0});
  auto ortho = ReplaceTransformation{renderContext.transformation(), projection, view};

  if (renderContext.render3D())
  {
    // Un-occluded handles: use depth test, draw fully opaque
    renderHandles(renderContext, m_pointHandles, m_handle, 1.0f);
    renderHandles(renderContext, m_highlights, m_highlight, 1.0f);

    // Occluded handles: don't use depth test, but draw translucent
    glAssert(glDisable(GL_DEPTH_TEST));
    renderHandles(renderContext, m_pointHandles, m_handle, 0.33f);
    renderHandles(renderContext, m_highlights, m_highlight, 0.33f);
    glAssert(glEnable(GL_DEPTH_TEST));
  }
  else
  {
    // In 2D views, render fully opaque without depth test
    glAssert(glDisable(GL_DEPTH_TEST));
    renderHandles(renderContext, m_pointHandles, m_handle, 1.0f);
    renderHandles(renderContext, m_highlights, m_highlight, 1.0f);
    glAssert(glEnable(GL_DEPTH_TEST));
  }

  clear();
}

void PointHandleRenderer::renderHandles(
  RenderContext& renderContext, const HandleMap& map, Circle& circle, const float opacity)
{
  const auto& camera = renderContext.camera();
  auto shader = ActiveShader{renderContext.shaderManager(), Shaders::HandleShader};

  for (const auto& [color, positions] : map)
  {
    shader.set("Color", mixAlpha(color, opacity));

    for (const auto& position : positions)
    {
      // In 3D view, nudge towards camera by the handle radius, to prevent lines (brush
      // edges, etc.) from clipping into the handle
      const auto nudgeTowardsCamera =
        renderContext.render3D()
          ? vm::normalize(camera.position() - position) * pref(Preferences::HandleRadius)
          : vm::vec3f{0, 0, 0};

      const auto offset =
        camera.project(position + nudgeTowardsCamera) * vm::vec3f{1, 1, -1};
      auto translate = MultiplyModelMatrix{
        renderContext.transformation(), vm::translation_matrix(offset)};
      circle.render();
    }
  }
}

void PointHandleRenderer::clear()
{
  m_pointHandles.clear();
  m_highlights.clear();
}

} // namespace tb::Renderer
