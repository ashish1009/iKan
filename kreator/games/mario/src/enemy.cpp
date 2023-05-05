//
//  enemy.cpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#include "enemy.hpp"

namespace mario {
  
  struct EnemyData {
  };
  
  struct EnemyScriptData {
  };
  static std::shared_ptr<EnemyScriptData> data;
  
  void EnemyScriptManager::Init() {
    data = std::make_shared<EnemyScriptData>();
  }
  void EnemyScriptManager::Shutdown() {
    data.reset();
  }
  
} // namespace mario
