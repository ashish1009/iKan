//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include "renderer_layer.hpp"

/// This Function should be defined in client side for creating client application derived from base ikan::Application
/// - Returns: Shared pointer of created application
extern std::unique_ptr<ikan::Application> CreateApplication();

class KreatorApp : public ikan::Application {
public:
  KreatorApp(const ikan::Application::Specification& application_spec, kreator::GameType game_type)
  : ikan::Application(application_spec), game_type_(game_type) {
    IK_TRACE("Kreator Application", "Creating Kreator Application Instance ...");
    
    // Disable the Logs module
    ikan::Logger::DisableModule(ikan::LogModule::CharTexture);
    
    // Create the instance of applciaiton based on the type of suppored applucaiton
    switch (game_type_) {
      case kreator::GameType::Mario :
        specification_.client_asset_path = "../../../kreator/mario/assets/";
        specification_.save_ini_file_path = "../../../kreator/mario/ini/mario.ini";
        specification_.window_specification.title = "Ikan Mario";
        break;
      case kreator::GameType::Editor :
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
    
    PushLayer(std::make_shared<kreator::RendererLayer>(game_type));
  }
  ~KreatorApp() {
    IK_TRACE("Kreator Application", "Destroying Kreator Application Instance !!!");
  }
  
  void OnRenderGui() override {
    ImGui::Begin("Kreator");
    ImGui::End();
  }
  
private:
  kreator::GameType game_type_;
};

/// This funtion implementatis the API for creating instance of Core::Application
std::unique_ptr<ikan::Application> CreateApplication() {
  // Set up all the applicaiton specification
  ikan::Application::Specification application_spec;

  application_spec.name = "I-Kan";
  application_spec.workspace_path = "../../../";

  // Platform
  application_spec.rendering_api = ikan::Renderer::Api::OpenGl;
  application_spec.os = ikan::OperatingSystem::Mac;
  
  // Utils
  
  // Window Specification
  application_spec.window_specification.title = "Untitled";
  application_spec.window_specification.width = 2100;
  application_spec.window_specification.height = 900;
  application_spec.window_specification.v_sync = true;
  application_spec.window_specification.fullscreen = false;
  application_spec.window_specification.hide_titlebar = false;
  
  application_spec.resizable = true;
  application_spec.start_maximized = false;
  
  return std::make_unique<KreatorApp>(application_spec, kreator::GameType::Editor);
}

int main() {
  // Initialize the ikan Logger
  ikan::Logger::Init(ikan::Logger::Level::Trace,
                     ikan::Logger::Level::Trace,
                     "../../../log/kreator.log");
  
  auto app = CreateApplication();
  if (app) {
    app->Run();
  }
  
  return 0;
}
