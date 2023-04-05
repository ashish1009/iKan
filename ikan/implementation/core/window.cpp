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

  void Window::Specification::Log() {
    IK_CORE_TRACE(LogModule::Window, "  Window Property ");
    IK_CORE_TRACE(LogModule::Window, "  ---------------------------------------------------------");
    IK_CORE_TRACE(LogModule::Window, "    Title      | {0}", title);
    IK_CORE_TRACE(LogModule::Window, "    Width      | {0}", width);
    IK_CORE_TRACE(LogModule::Window, "    Height     | {0}", height);
    IK_CORE_TRACE(LogModule::Window, "    VSync      | {0}", v_sync);
    IK_CORE_TRACE(LogModule::Window, "    FullScreen | {0}", fullscreen);
    IK_CORE_TRACE(LogModule::Window, "    Hide Title | {0}", hide_titlebar);
    IK_CORE_TRACE(LogModule::Window, "  ---------------------------------------------------------");
  }

} // namespace ikan
