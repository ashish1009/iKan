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

    RendererData(Renderer::Api new_api) : api(new_api) {
      IK_CORE_INFO(LogModule::Renderer, "Setting the Renderer API as {0} ...", renderer_utils::GetRendererApiName(api));
    }
    DELETE_COPY_MOVE_CONSTRUCTORS(RendererData);
  };
  static std::unique_ptr<RendererData> renderer_data;
  
  void Renderer::CreateRendererData(Api api) {
    renderer_data = std::make_unique<RendererData>(api);
  }
  void Renderer::Initialize() {
    
  }
  void Renderer::Shutdown() {
  }
  
  Renderer::Api Renderer::GetApi() { return renderer_data->api; }

  Renderer::Capabilities& Renderer::Capabilities::Get() {
    static Capabilities capabilities;
    return capabilities;
  }
  
  void Renderer::Capabilities::Log() {
    IK_CORE_INFO(LogModule::Renderer, "  Renderer Capability ");
    IK_CORE_INFO(LogModule::Renderer, "    Vendor   | {0} ", vendor);
    IK_CORE_INFO(LogModule::Renderer, "    Renderer | {0} ", renderer);
    IK_CORE_INFO(LogModule::Renderer, "    Version  | {0} ", version);
  }
  
} // namespace ikan
