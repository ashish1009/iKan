//
//  open_gl_renderer_api.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "open_gl_renderer_api.hpp"

namespace ikan {
  
  OpenGLRendererAPI::OpenGLRendererAPI() {
    IK_CORE_TRACE(LogModule::Renderer, "Creating Open GL Renderer API ...");
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI() noexcept {
    IK_CORE_WARN(LogModule::Renderer, "Destroying Open GL Renderer API !!!");
  }

} // namespace ikan
