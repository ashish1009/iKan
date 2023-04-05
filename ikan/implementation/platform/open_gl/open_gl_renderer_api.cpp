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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    
    auto& caps = Renderer::Capabilities::Get();
    
    caps.vendor   = (const char*)glGetString(GL_VENDOR);
    caps.renderer = (const char*)glGetString(GL_RENDERER);
    caps.version  = (const char*)glGetString(GL_VERSION);
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI() {
    IK_CORE_TRACE(LogModule::Renderer, "Destroying Open GL Renderer API !!!");
  }
  
} // namespace ikan
