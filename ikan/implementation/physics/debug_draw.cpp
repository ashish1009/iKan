//
//  debug_draw.cpp
//  ikan
//
//  Created by Ashish . on 11/05/23.
//

#include "debug_draw.hpp"
#include "renderer/utils/batch_2d_renderer.hpp"

namespace ikan {
    
  void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    
  }
  
  void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    for (int32_t i = 0; i < vertexCount; i++) {
      if (i + 1 < vertexCount)
        Batch2DRenderer::DrawLine({vertices[i].x, vertices[i].y, 0.01}, {vertices[i + 1].x, vertices[i + 1].y, 0.01}, {color.r, color.g,color.b, color.a});
      else
        Batch2DRenderer::DrawLine({vertices[i].x, vertices[i].y, 0.01}, {vertices[0].x, vertices[0].y, 0.01}, {color.r, color.g,color.b, color.a});
    }
  }
  
  void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {

  }
  
  void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
    Batch2DRenderer::DrawCircle({center.x, center.y, 0.01}, {2 * radius, 2 * radius, 0}, {0, 0, 0}, {color.r, color.g,color.b, color.a}, 0.05);
  }
  
  void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    
  }
  
  void DebugDraw::DrawTransform(const b2Transform& xf) {
    
  }
  
  void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
    
  }
  
} // namespace ikan
