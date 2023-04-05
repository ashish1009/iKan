//
//  core_application.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/window.hpp"
#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  class Application;
  
  /// Defination API for creating application instance.
  /// - Note: only defination is defined in core. Declaration should be at client side based on the application they want to create
  std::unique_ptr<Application> CreateApplication();
  
  /// This class is the base class of any application that will be running in the client side. This class is responsible for
  ///   - Creating Window for the current supported OS
  ///   - Initialize the renderer for the current renderer API
  ///   - Add and store Layer for rendering or any other operations
  ///   - Render the GUI window
  ///   - Handle all the events of the windows Idly events should be handled at Window class only but functionlity depends on the application
  ///     (passed as function pointer)
  class Application {
  public:
    /// This structure holds the specification of application provided by client
    struct Specification {
      std::string name = "iKan";
      std::string client_asset_path;
      std::string save_ini_file_path;
      Renderer::Api rendering_api = Renderer::Api::None;
      OperatingSystem os = OperatingSystem::None;
      Window::Specification window_specification;
      bool start_maximized = true;
      bool resizable = true;
      
      void Log();
    };
    
  };
    
} // namespace ikan
