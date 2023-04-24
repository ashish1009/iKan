//
//  runtime_items.hpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class Items {
    Coin, Mushroom, Flower, Fireball, Star, Score
  };
  
  class CoinController : public ScriptableEntity {
  };
  
  class MushroomController : public ScriptableEntity {
  };
  
  class FlowerController : public ScriptableEntity {
  };
  
  class FireballController : public ScriptableEntity {
  };
  
  class StarController : public ScriptableEntity {
  };
  
  class ScoreController : public ScriptableEntity {
  };
  
  class RuntimeItem {
  public:
    /// This function initialise the runtime item scripts and data
    static void Init();
    /// This function sutdowns the runtime item scripts and data
    static void Shutdown();
  };
  
} // namespace mario
