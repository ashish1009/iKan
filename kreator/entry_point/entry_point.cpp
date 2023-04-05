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
  auto core_level   = ikan::Logger::Level::Trace;
  auto client_level = ikan::Logger::Level::Trace;
  
  ikan::Logger::Init(core_level, client_level, "../../../log/kreator.log");
  
  auto app = CreateApplication();
  if (app) {
    app->Run();
  }
  
  return 0;
}
