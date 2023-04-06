//
//  renderer_buffer.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "renderer_buffer.hpp"
#include "platform/open_gl/open_gl_renderer_buffer.hpp"

namespace ikan {
  
  // --------------------------------------------------------------------------
  // Vertex Buffer
  // --------------------------------------------------------------------------
  std::shared_ptr<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLVertexBuffer>(data, size);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLVertexBuffer>(size);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
} // namespace ikan
