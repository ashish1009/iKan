//
//  mac_window.cpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#include "mac_window.hpp"

namespace ikan {
  
  MacWindow::MacWindow(const Specification& window_spec) {
    IK_CORE_INFO(LogModule::Window, "Creating MAC OS Window instacne ... ");
    
    // Move the specificaion to MacWindow data
    mac_window_data_.specification = window_spec;
    mac_window_data_.specification.Log();
  }
  MacWindow::~MacWindow() {
    
  }
  
} // namespace ikan
