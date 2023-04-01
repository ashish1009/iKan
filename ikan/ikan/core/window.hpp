//
//  window.hpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/events/event.h"

namespace ikan {
  
  /// This class is the abstract class for creating Window instance. This class defines all the API to be used by core or client.
  /// - Note: To Create instance of window Call the static API 'Window::Create' with Operating system type and specificaion of windows
  class Window {
  public:
    /// This structure holds the specification for window.
    struct Specification {
      std::string title = "ikan Window";
      uint32_t width = 1600, height = 900;

      bool v_sync = true;
      bool fullscreen = false;
      bool hide_titlebar = false;

      void Log();
    };

    /// This function create instance of to create a window based on the Operating system supported by the sytem / Engine. This is resposible to create
    /// the GLFW Window instance for the OS and Create the Renderer Context for the suppoorted Renderer API.
    /// - Parameters:
    ///   - os: Current operating system
    ///   - specificaiton: window specification
    /// - Important: Set the renderer API befiore creating the Widnow, as it is needed to creaste the Contexrt
    static std::unique_ptr<Window> Create(OperatingSystem os, const Specification& spec);

    /// Default virtual destructor
    virtual ~Window() noexcept = default;
  };
  
} // namespace ikan
