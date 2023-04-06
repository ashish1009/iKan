//
//  renderer.cpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#include "renderer.hpp"
#include "batch_2d_renderer.hpp"
#include "text_renderer.hpp"
#include "renderer_stats.hpp"
#include "shader.hpp"

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
      IK_CORE_TRACE(LogModule::Renderer, "Setting the Renderer API as {0} ...", renderer_utils::GetRendererApiName(api));
    }
    DELETE_COPY_MOVE_CONSTRUCTORS(RendererData);
  };
  static std::unique_ptr<RendererData> renderer_data_;
  
  // -------------------------------------------------------------------------
  // Fundamentals
  // -------------------------------------------------------------------------
  void Renderer::CreateRendererData(Api api) {
    renderer_data_ = std::make_unique<RendererData>(api);
  }
  void Renderer::Initialize() {
    renderer_data_->renderer_api_instance = RendererAPI::Create();
    
    Batch2DRenderer::Initialise(100, 100, 100);
    TextRenderer::Initialise();
  }
  void Renderer::Shutdown() {
    renderer_data_.reset();
    
    Batch2DRenderer::Shutdown();
    TextRenderer::Shutdown();
    
    ShaderLibrary::ResetShaders();
  }
  
  // -------------------------------------------------------------------------
  // Fundamental Renderer API
  // -------------------------------------------------------------------------
  Renderer::Api Renderer::GetApi() { return renderer_data_->api; }
  void Renderer::Clear(const glm::vec4& color) {
    renderer_data_->renderer_api_instance->SetClearColor(color);
    renderer_data_->renderer_api_instance->ClearBits();
  }
  void Renderer::SetClearColor(const glm::vec4& color) {
    renderer_data_->renderer_api_instance->SetClearColor(color);
  }
  void Renderer::ClearBits() {
    renderer_data_->renderer_api_instance->ClearBits();
  }
  void Renderer::ClearDepthBit() {
    renderer_data_->renderer_api_instance->ClearDepthBit();
  }
  void Renderer::ClearColorBit() {
    renderer_data_->renderer_api_instance->ClearColorBit();
  }
  void Renderer::ClearStencilBit() {
    renderer_data_->renderer_api_instance->ClearStencilBit();
  }
  void Renderer::Depth(bool state) {
    renderer_data_->renderer_api_instance->Depth(state);
  }
  void Renderer::Blend(bool state) {
    renderer_data_->renderer_api_instance->Blend(state);
  }
  void Renderer::MultiSample(bool state) {
    renderer_data_->renderer_api_instance->MultiSample(state);
  }
  void Renderer::DepthFunc(RendererAPI::GlFunc func) {
    renderer_data_->renderer_api_instance->DepthFunc(func);
  }
  void Renderer::BeginWireframe() {
    renderer_data_->renderer_api_instance->BeginWireframe();
  }
  void Renderer::EndWireframe() {
    renderer_data_->renderer_api_instance->EndWireframe();
  }
  
  void Renderer::SetViewport(uint32_t width, uint32_t height) {
    renderer_data_->renderer_api_instance->SetViewport(width, height);
  }
  void Renderer::GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixel_id_index, int32_t& pixel_data) {
    renderer_data_->renderer_api_instance->GetEntityIdFromPixels(mx, my, pixel_id_index, pixel_data);
  }

  // -------------------------------------------------------------------------
  // Renderer Stats API
  // -------------------------------------------------------------------------
  void Renderer::ResetStatsEachFrame() { RendererStatistics::Get().ResetEachFrame(); }
  void Renderer::ResetStats() { RendererStatistics::Get().ResetAll(); }
  void Renderer::RenderStatsGui(bool *is_open) { RendererStatistics::Get().RenderGui(is_open); }
  void Renderer::Render2DStatsGui(bool *is_open) { RendererStatistics::Get().stats_2d_.RenderGui(is_open); }

  // -------------------------------------------------------------------------
  // Library
  // -------------------------------------------------------------------------
  std::shared_ptr<Shader> Renderer::GetShader(const std::string& path) { return ShaderLibrary::GetShader(path); }

  // -------------------------------------------------------------------------
  // Renderer Capabilities
  // -------------------------------------------------------------------------
  Renderer::Capabilities& Renderer::Capabilities::Get() {
    static Capabilities capabilities;
    return capabilities;
  }
  
  void Renderer::Capabilities::Log() {
    IK_CORE_TRACE(LogModule::Renderer, "  Renderer Capability ");
    IK_CORE_TRACE(LogModule::Renderer, "    Vendor   | {0} ", vendor);
    IK_CORE_TRACE(LogModule::Renderer, "    Renderer | {0} ", renderer);
    IK_CORE_TRACE(LogModule::Renderer, "    Version  | {0} ", version);
  }
  
} // namespace ikan
