//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include <iostream>

int main() {
  ikan::Logger::Init(ikan::Logger::Level::Trace, ikan::Logger::Level::Trace, "../../../log/kreator.log");
  
  IK_CORE_INFO(ikan::LogModule::None, "Testing Log");
  IK_INFO(ikan::LogModule::None, "Testing Log");
  
  IK_ASSERT(false);
  
  return 0;
}
