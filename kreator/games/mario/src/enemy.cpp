//
//  enemy.cpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#include "enemy.hpp"

namespace mario {
  
  struct EnemyData {
    EnemyType type;
    ScriptLoaderFn loader_fun;
    EnemyData() = default;
    EnemyData(EnemyType type, ScriptLoaderFn fun) : type(type), loader_fun(fun) { }
  };
  
  struct EnemyScriptData {
    std::unordered_map<EnemyType, EnemyData> script_map;
  };
  static std::shared_ptr<EnemyScriptData> data;
  
  void EnemyScriptManager::Init() {
    data = std::make_shared<EnemyScriptData>();
    
    static auto goomba_load_fn = ScriptLoader(mario::GoombaController);
    static auto duck_load_fn = ScriptLoader(mario::DuckController);
    
    data->script_map[EnemyType::Goomba] = {EnemyType::Goomba, goomba_load_fn};
    data->script_map[EnemyType::Duck] = {EnemyType::Duck, duck_load_fn};
  }
  void EnemyScriptManager::Shutdown() {
    data.reset();
  }
  
} // namespace mario
