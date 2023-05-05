//
//  enemy.hpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class EnemyType {
    Goomba, Duck
  };
  
  enum class EnemyState {
    Alive, Dying, Dead
  };
  
  class GoombaController : public ScriptableEntity {
  };

  class DuckController : public ScriptableEntity {
    
  };

  class EnemyScriptManager {
  public:
    /// This funciton initialise the Enemy Scripts
    static void Init();
    /// This function shuts destroy the Enemy scripts
    static void Shutdown();
  };
  using ESM = EnemyScriptManager;
  
} // namespace mario
