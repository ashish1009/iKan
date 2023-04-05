//
//  kreator_application.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

/// This funtion implementatis the API for creating instance of Core::Application
std::unique_ptr<ikan::Application> CreateApplication() {
  ikan::Application::Specification application_spec;
  
  // Platform
  application_spec.rendering_api = ikan::Renderer::Api::OpenGl;
  application_spec.os = ikan::OperatingSystem::Mac;
  
  // Utils
  application_spec.name = "Kreator Application";
  application_spec.client_asset_path = "../../../kreator/assets/";
  application_spec.save_ini_file_path = "../../../kreator/editor/ini/editor.ini";
  
  // Window Specification
  application_spec.window_specification.title = "Untitled Window";
  application_spec.window_specification.width = 2100;
  application_spec.window_specification.height = 900;
  application_spec.window_specification.v_sync = true;
  application_spec.window_specification.fullscreen = false;
  application_spec.window_specification.hide_titlebar = false;
  
  application_spec.resizable = true;
  application_spec.start_maximized = false;

  return std::make_unique<ikan::Application>(application_spec);
}
