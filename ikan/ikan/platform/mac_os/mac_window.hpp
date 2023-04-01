//
//  mac_window.hpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/window.hpp"
#include <GLFW/glfw3.h>

namespace ikan {
  
  /// This class is responsble for creating Window for MAC OS using GLFW
  class MacWindow : public Window {
  public:
    /// Mac window Data
    struct Data {
      Window::Specification specification;
      EventCallbackFn event_callback_function;
    };

    /// This constructuor creates an GLFW window for MAC OS. Register the data in window set back pointer
    /// - parameter specification: window specification
    MacWindow(const Specification& window_spec);
    /// Destroy the GLFW window and shutdown.
    ~MacWindow();
    
  private:
    /// This function sets the event callback fucntions to window data
    void SetEventCallbacks();

    GLFWwindow* window_;
    Data mac_window_data_;
  };
  
} // namespace ikan
