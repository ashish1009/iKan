//
//  renderer_api.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "renderer_api.hpp"
#include "renderer/utils/renderer.hpp"
#include "platform/open_gl/open_gl_renderer_api.hpp"

namespace ikan {
  
  std::unique_ptr<RendererAPI> RendererAPI::Create() {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl : return std::make_unique<OpenGLRendererAPI>();
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Render API not Supporting");
    }
  }
  
} // namespace ikan
