//
//  renderer_layer.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#include "renderer_layer.hpp"

namespace kreator {
  
#define KREATOR_LOG(...) IK_TRACE(game_data_->GameName(), __VA_ARGS__)
  
  RendererLayer::RendererLayer(GameType game_type) : Layer("Kreator"), game_data_(CreateGameData(game_type)) {
    KREATOR_LOG("Creating {0} Layer instance ... ", game_data_->GameName().c_str());
    
    Batch2DRenderer::AddQuadData(100);
    Batch2DRenderer::AddQuadData(100);
  }
  
  RendererLayer::~RendererLayer() {
    KREATOR_LOG("Destroying {0} Layer instance !!! ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Attach() {
    KREATOR_LOG("Attaching {0} Layer instance", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Detach() {
    KREATOR_LOG("Detaching {0} Layer instance ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Update(Timestep ts) {
    Renderer::Clear(game_data_->GetBgColor());
  }
  
  void RendererLayer::HandleEvents(Event& event) {
  }
    
  void RendererLayer::RenderGui() {
    if (is_playing_) {
    }
    else {
      ImguiAPI::StartDcocking();

      ShowMenu();

      ImguiAPI::EndDcocking();
    }
  }
  
  void RendererLayer::ShowMenu() {
    if (ImGui::BeginMenuBar()) {
      ImguiAPI::Menu("File", true, []() {
        ImguiAPI::Menu("Scene", false, []() {
          
        }); // Scene
        ImGui::Separator();
        ImguiAPI::MenuItem("Exit", "Cmd + Q", false, true, []() {
          Application::Get().Close();
        }); // Exit
      }); // File
      ImguiAPI::Menu("Property", true, []() {
        ImguiAPI::Menu("Theme", false, []() {
          
        }); // Theme
      }); // Property
      
      ImguiAPI::Menu("Settings", false, []() {
      }); // Settings
      ImGui::EndMenuBar(); // ImGui::BeginMenuBar()
    } // if (ImGui::BeginMenuBar())
  }
  
} // namespace kreator