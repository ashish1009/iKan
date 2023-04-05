//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

/// This Function should be defined in client side for creating client application derived from base ikan::Application
/// - Returns: Shared pointer of created application
extern std::unique_ptr<ikan::Application> CreateApplication();

int main() {
  // Initialize the ikan Logger
  auto core_level   = ikan::Logger::Level::Info;
  auto client_level = ikan::Logger::Level::Info;
  
  ikan::Logger::Init(core_level, client_level, "../../../log/kreator.log");
  
#ifdef IK_DEBUG_FEATURE
  auto spd_core_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(core_level);
  auto spd_client_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(client_level);
  
  IK_TRACE("Core Entry Point", "Initialized the spd logger ");
  IK_TRACE("Core Entry Point", "  Core   | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_core_log_level));
  IK_TRACE("Core Entry Point", "  Client | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_client_log_level));
#endif
  
  auto app = CreateApplication();
  if (app) {
    app->Run();
  }
  
  return 0;
}
