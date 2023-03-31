//
//  renderer.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "renderer.hpp"

namespace ikan {
  
  namespace renderer_utils {
    
#ifdef IK_DEBUG_FEATURE
    std::string GetRendererApiName(Renderer::Api api) {
      switch (api) {
        case Renderer::Api::OpenGl: return "Open GL";
        case Renderer::Api::None:
        default:
          IK_CORE_ASSERT(false, "Invalid Renderer API");
      }
    }
#endif
    
  } // namespace renderer_utils

  struct RendererData {
    Renderer::Api api = Renderer::Api::None;
    
    RendererData(Renderer::Api new_api) : api(new_api) {
      IK_CORE_TRACE(LogModule::Renderer, "Creating Renderer Data Instance with {0} API ...", renderer_utils::GetRendererApiName(api));
    }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(RendererData);
  };
  static std::unique_ptr<RendererData> renderer_data;
  
  // -------------
  // Fundamentals
  // -------------
  void Renderer::Initialize(Api api) {
    renderer_data = std::make_unique<RendererData>(api);
  }
  void Renderer::Shutdown() {
    
  }
  
} // namespace ikan
