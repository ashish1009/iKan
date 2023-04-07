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
  KreatorApp(const ikan::Application::Specification& application_spec, kreator::GameType game_type):
  ikan::Application(application_spec) {
    ikan::Logger::DisableModule(ikan::LogModule::CharTexture);
    
    IK_TRACE("Kreator Application", "Creating Kreator Application Instance ...");
    PushLayer(std::make_shared<kreator::RendererLayer>(game_type));
  }
  ~KreatorApp() {
    IK_TRACE("Kreator Application", "Destroying Kreator Application Instance !!!");
  }
};

/// This funtion implementatis the API for creating instance of Core::Application
std::unique_ptr<ikan::Application> CreateApplication() {
  // Type of game to be run on application
  kreator::GameType game_type = kreator::GameType::Editor;

  // Set up all the applicaiton specification
  ikan::Application::Specification application_spec;
  
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
  
  // Create the instance of applciaiton based on the type of suppored applucaiton
  switch (game_type) {
    case kreator::GameType::Editor :
      application_spec.name = "IKan";
      application_spec.workspace_path = "../../../";
      application_spec.client_asset_path = "../../../kreator/editor/assets/";
      application_spec.save_ini_file_path = "../../../kreator/editor/ini/editor.ini";

      application_spec.window_specification.title = "Kreator";
      break;
      
    default:
      break;
  };

  return std::make_unique<KreatorApp>(application_spec, game_type);
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
