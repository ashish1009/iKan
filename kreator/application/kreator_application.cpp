//
//  kreator_application.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

/// This funtion implementatis the API for creating instance of Core::Application
std::unique_ptr<ikan::Application> CreateApplication() {
  return std::make_unique<ikan::Application>();
}
