//
//  graphics_context.cpp
//  ikan
//
//  Created by Ashish . on 03/04/23.
//

#include "graphics_context.hpp"

#include "renderer/utils/renderer.hpp"
#include "platform/open_gl/open_gl_graphics_context.hpp"

namespace ikan {
  
  std::unique_ptr <GraphicsContext> GraphicsContext::Create(GLFWwindow *window) {
    switch(Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_unique<OpenGLGraphicsContext>(window);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API which is not supported by ikan engine" );
    }
  }
  
} // namespace ikan
