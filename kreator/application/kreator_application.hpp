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
    : ikan::Application(application_spec), game_type_(game_type) {
      IK_TRACE("Kreator Application", "Creating Kreator Application Instance ...");
      CreateLayer();
    }
    ~KreatorApp() {
      IK_TRACE("Kreator Application", "Destroying Kreator Application Instance !!!");
    }
    
    void OnRenderGui() override {
      ImGui::Begin("Kreator");
      ImGui::End();
    }
    
  private:
    void CreateLayer() {
      // Disable the Logs module
      ikan::Logger::DisableModule(ikan::LogModule::CharTexture);
      
      // Create the instance of applciaiton based on the type of suppored applucaiton
      switch (game_type_) {
        case GameType::Mario :
          specification_.client_asset_path = "../../../kreator/mario/assets/";
          specification_.save_ini_file_path = "../../../kreator/mario/ini/mario.ini";
          specification_.window_specification.title = "Ikan Mario";
          break;
        case GameType::Editor :
          specification_.client_asset_path = "../../../kreator/editor/assets/";
          specification_.save_ini_file_path = "../../../kreator/editor/ini/editor.ini";
          specification_.window_specification.title = "Kreator";
          break;
          
        default:
          break;
      };
      
      // Update the Application based on game selected
      ikan::DirectoryManager::SetClientAssetPath(specification_.client_asset_path);
      imgui_layer_->SetIniFilePath(specification_.save_ini_file_path);
      window_->SetTitle(specification_.window_specification.title);
      
      PushLayer(std::make_shared<RendererLayer>(game_type_));
    }
    
    GameType game_type_;
  };

} // namespace kreator
