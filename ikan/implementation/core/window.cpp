//
//  window.cpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#include "window.hpp"
#include "platform/mac_os/mac_window.hpp"

namespace ikan {
  
  std::shared_ptr<Window> Window::Create(OperatingSystem os, const Specification& specificaiton) {
    switch (os) {
      case OperatingSystem::Mac: return std::make_shared<MacWindow>(specificaiton);
      case OperatingSystem::None:
      default:
        IK_CORE_ASSERT(false, "Provided OS Not Suported !!!");
    }
  }

} // namespace ikan
