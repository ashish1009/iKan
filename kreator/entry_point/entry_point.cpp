//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include <iostream>

int main() {
  // Initialize the ikan Logger
  auto core_level   = ikan::Logger::Level::Trace;
  auto client_level = ikan::Logger::Level::Trace;
  
  ikan::Logger::Init(core_level, client_level, "../../../log/kreator.log");
  
#ifdef IK_DEBUG_FEATURE
  auto spd_core_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(core_level);
  auto spd_client_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(client_level);
  
  IK_INFO("Core Entry Point", "Initialized the spd logger ");
  IK_INFO("Core Entry Point", "  Core   | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_core_log_level));
  IK_INFO("Core Entry Point", "  Client | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_client_log_level));
#endif
  
  {
    ikan::Renderer::CreateRendererData(ikan::Renderer::Api::OpenGl);
    // Create Window
    ikan::Renderer::Initialize();
  }
  
  {
    ikan::Renderer::Shutdown();
  }
  
  return 0;
}
