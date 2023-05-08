//
//  kreator_application.hpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#pragma once

#include "renderer_layer.hpp"

namespace kreator {
  
  class KreatorApp : public ikan::Application {
  public:
    KreatorApp(const ikan::Application::Specification& application_spec, GameType game_type)
    : ikan::Application(application_spec), current_game_type_(game_type) {
      IK_TRACE("Kreator Application", "Creating Kreator Application Instance ...");
      CreateLayer();
    }
    virtual ~KreatorApp() {
      IK_TRACE("Kreator Application", "Destroying Kreator Application Instance !!!");
    }
    
    void OnUpdate(Timestep ts) override {
      if (change_game_) {
        PopLayer(renderer_layer_);
        CreateLayer();
        change_game_ = false;
      }
    }
    
    void OnRenderGui() override {
      if (!renderer_layer_->IsPlaying()) {
        ImGui::Begin("Kreator", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        GameType current_type = current_game_type_;
        GameType new_type = GameType(PropertyGrid::ComboDrop("Kreator Game Type",
                                                             { "Editor" , "Mario", "Chess", "Angry Bird", "Commando" }, (uint32_t)current_type,
                                                             3 * ImGui::GetWindowContentRegionMax().x / 5));
        if (new_type != current_game_type_) {
          current_game_type_ = new_type;
          change_game_ = true;
        }
        ImGui::End();
      }
    }
    
  private:
    void CreateLayer() {
      // Disable the Logs module
      ikan::Logger::DisableModule(ikan::LogModule::CharTexture);
      
      // Create the instance of applciaiton based on the type of suppored applucaiton
      switch (current_game_type_) {
        case GameType::Editor :
          specification_.window_specification.title = "Kreator";
          specification_.client_asset_path = "../../../kreator/games/editor/assets/";
          break;
        case GameType::Mario :
          specification_.window_specification.title = "Ikan Mario";
          specification_.client_asset_path = "../../../kreator/games/mario/assets/";
          break;
        case GameType::Chess :
          specification_.window_specification.title = "Ikan Chess";
          specification_.client_asset_path = "../../../kreator/games/chess/assets/";
          break;
        case GameType::AngryBird :
          specification_.window_specification.title = "Ikan Angry Bird";
          specification_.client_asset_path = "../../../kreator/games/angry_bird/assets/";
          break;
        case GameType::Commando :
          specification_.window_specification.title = "Ikan Commando";
          specification_.client_asset_path = "../../../kreator/games/commando/assets/";
          break;

        default:
          break;
      };
      
      // Update the Application based on game selected
      ikan::DirectoryManager::SetClientAssetPath(specification_.client_asset_path);
      imgui_layer_->SetIniFilePath(specification_.save_ini_file_path);
      window_->SetTitle(specification_.window_specification.title);
      
      renderer_layer_ = std::make_shared<RendererLayer>(current_game_type_);
      PushLayer(renderer_layer_);
    }
    
    bool change_game_ = false;
    GameType current_game_type_;
    std::shared_ptr<RendererLayer> renderer_layer_;
  };

} // namespace kreator
