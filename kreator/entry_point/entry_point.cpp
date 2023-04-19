//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include "kreator_application.hpp"

/// This Function should be defined in client side for creating client application derived from base ikan::Application
/// - Returns: Shared pointer of created application
extern std::unique_ptr<ikan::Application> CreateApplication();

/// This funtion implementatis the API for creating instance of Core::Application
std::unique_ptr<ikan::Application> CreateApplication() {
  // Set up all the applicaiton specification
  ikan::Application::Specification application_spec;

  application_spec.name = "I-Kan";
  application_spec.workspace_path = "../../../";

  // Platform
  application_spec.rendering_api = ikan::Renderer::Api::OpenGl;
  application_spec.os = ikan::OperatingSystem::Mac;
  
  // Window Specification
  application_spec.window_specification.title = "Untitled";
  application_spec.window_specification.width = 2100;
  application_spec.window_specification.height = 900;
  application_spec.window_specification.v_sync = true;
  application_spec.window_specification.fullscreen = false;
  application_spec.window_specification.hide_titlebar = false;
  
  application_spec.resizable = true;
  application_spec.start_maximized = false;
  
  return std::make_unique<kreator::KreatorApp>(application_spec, kreator::GameType::Mario);
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
