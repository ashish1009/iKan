//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include <iostream>

class Temp : public ikan::RefCounted {
public:
  int a, b, c;
};

int main() {
  ikan::Logger::Init(ikan::Logger::Level::Trace, ikan::Logger::Level::Trace, "../../../log/kreator.log");
  
  IK_CORE_INFO(ikan::LogModule::None, "Testing Log");
  IK_INFO(ikan::LogModule::None, "Testing Log");
  
  ikan::Ref<Temp> t = ikan::Ref<Temp>::Create();
  
  IK_ASSERT(false);
  
  return 0;
}
