//
//  renderer.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "renderer.hpp"
#include "renderer/graphics/renderer_api.hpp"

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
    std::unique_ptr<RendererAPI> renderer_api_instance;

    RendererData(Renderer::Api new_api) : api(new_api) { }
    DELETE_COPY_MOVE_CONSTRUCTORS(RendererData);
  };
  static std::unique_ptr<RendererData> renderer_data;
  
  void Renderer::Initialize(Api api) {
    renderer_data = std::make_unique<RendererData>(api);
    renderer_data->renderer_api_instance = RendererAPI::Create();
  }
  void Renderer::Shutdown() {
    renderer_data.reset();
  }
  
  Renderer::Api Renderer::GetApi() { return renderer_data->api; }
  
} // namespace ikan
