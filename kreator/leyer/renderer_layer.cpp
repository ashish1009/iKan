//
//  renderer_layer.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#include "renderer_layer.hpp"

namespace kreator {
  
#define KREATOR_LOG(...) IK_TRACE(game_data_->GameName(), __VA_ARGS__)
  
#define FOR_EACH_SETTING \
  SettingWrapper* setting_data = ((SettingWrapper*)&setting_); \
  static uint32_t loop_limit = sizeof(Setting) / sizeof(SettingWrapper); \
  for (uint32_t setting_idx = 0; setting_idx < loop_limit; setting_idx++) \

  
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
  
    Table tt(4);
    tt.AddColumn({"Name", "Type", "Normalized", "Size", "Offset"});
    tt.AddColumn({"Positiom", "Float3", "true", "10", "100"});
    tt.AddColumn({"Normal", "Float2", "false", "20", "90"});
    tt.AddColumn({"TexCoords", "Float2", "true", "30", "0"});
    
    tt.Dump();
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
      ShowSettings();
      
      ImguiAPI::Framerate(&setting_.frame_rate.flag);
      Renderer::RenderStatsGui(&setting_.common_renderer_stats.flag);
      Renderer::Render2DStatsGui(&setting_.renderer_stats_2d.flag);

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
      
      ImguiAPI::Menu("Settings", true, [this]() {
        FOR_EACH_SETTING {
          (setting_data + setting_idx)->ShowInMenu();
        }
      }); // Settings
      
      ImGui::EndMenuBar(); // ImGui::BeginMenuBar()
    } // if (ImGui::BeginMenuBar())
  }
  
  void RendererLayer::ShowSettings() {
    ImGui::Begin("Settings");

    FOR_EACH_SETTING {
      (setting_data + setting_idx)->CheckBox();
    }

    ImGui::End();
  }
  
} // namespace kreator
