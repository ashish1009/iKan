//
//  open_gl_renderer_api.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "open_gl_renderer_api.hpp"
#include "renderer/utils/renderer.hpp"

#include <glad/glad.h>

namespace ikan {
  
  OpenGLRendererAPI::OpenGLRendererAPI() {
    IK_CORE_TRACE(LogModule::Renderer, "Creating Open GL Renderer API ...");
    Initialise();
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI() noexcept {
    IK_CORE_WARN(LogModule::Renderer, "Destroying Open GL Renderer API !!!");
  }

  void OpenGLRendererAPI::Initialise() const {
    IK_CORE_INFO(LogModule::Renderer, "Initializeing Open GL Renderer API");
  }
  
  void OpenGLRendererAPI::Shutdown() const {
    IK_CORE_WARN(LogModule::Renderer, "Shutting down Open GL Renderer API");
  }

} // namespace ikan
