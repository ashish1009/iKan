//
//  core_application.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "core_application.hpp"

namespace ikan {

  namespace applicaiton_utils {
    
#ifdef IK_DEBUG_FEATURE
    /// This function returns the Operating System Type as string
    /// - Parameter os: OS Type in iKan Enum
    static std::string GetOsNameAsString(OperatingSystem os) {
      switch (os) {
        case OperatingSystem::Mac : return "MAC OS";
        case OperatingSystem::None :
        default:
          IK_ASSERT(false, "Invalid OS Type");
      }
    }
#endif
    
  } // namespace applicaiton_utils

  void Application::Specification::Log() {
    IK_CORE_INFO(LogModule::Application, "  Application Property ");
    IK_CORE_INFO(LogModule::Application, "  ---------------------------------------------------------");
    IK_CORE_INFO(LogModule::Application, "    Name                 | {0}", name);
    IK_CORE_INFO(LogModule::Application, "    Client Asset Path    | {0}", client_asset_path.c_str());
    IK_CORE_INFO(LogModule::Application, "    Save INI File Path   | {0}", save_ini_file_path.c_str());
    IK_CORE_INFO(LogModule::Application, "    Rewndering API       | {0}", renderer_utils::GetRendererApiName(rendering_api));
    IK_CORE_INFO(LogModule::Application, "    Operating System     | {0}", applicaiton_utils::GetOsNameAsString(os));
    IK_CORE_INFO(LogModule::Application, "    Window Maximized     | {0}", start_maximized);
    IK_CORE_INFO(LogModule::Application, "    Window Resizable     | {0}", resizable);
    IK_CORE_INFO(LogModule::Application, "  ---------------------------------------------------------");
  }

} // namespace ikan
