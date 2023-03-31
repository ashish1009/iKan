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
    
    // API for Text enable
    IK_CORE_INFO(LogModule::Renderer, "  Asignment           | {0} (Unpacked)", true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Multi Sample
    IK_CORE_INFO(LogModule::Renderer, "  Multi Sample Feild  | {0}", true);
    glEnable(GL_MULTISAMPLE);
    
    // Blending Teting
    IK_CORE_INFO(LogModule::Renderer, "  Blending Test Feild | {0}", true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Depth Testing
    IK_CORE_INFO(LogModule::Renderer, "  Depth Test Feild    | {0}", true);
    glEnable(GL_DEPTH_TEST);
    
    auto& caps = Renderer::Capabilities::Get();
    
    caps.vendor   = (const char*)glGetString(GL_VENDOR);
    caps.renderer = (const char*)glGetString(GL_RENDERER);
    caps.version  = (const char*)glGetString(GL_VERSION);
  }
  
  void OpenGLRendererAPI::Shutdown() const {
    IK_CORE_WARN(LogModule::Renderer, "Shutting down Open GL Renderer API");
  }

} // namespace ikan
