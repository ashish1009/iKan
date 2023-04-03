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
  
  std::unique_ptr<ikan::Window> window_;
  {
    ikan::Renderer::CreateRendererData(ikan::Renderer::Api::OpenGl);
    
    window_ = ikan::Window::Create(ikan::OperatingSystem::Mac, ikan::Window::Specification());
    
    ikan::Renderer::Initialize();

    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                     Core Application Initialized                         ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }

  {
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                          Starting Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");

    while (1) {
      window_->Update();
    }
    
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                            Ending Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }
  {
    ikan::Renderer::Shutdown();
  }
  
  return 0;
}
