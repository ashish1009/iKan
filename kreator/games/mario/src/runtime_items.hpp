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
    Coin, Mushroom, Flower, Score, Fireball, BlastFireball
  };
  
  class RuntimeItem {
  public:
    /// This function initialise the runtime item scripts and data
    static void Init();
    /// This function sutdowns the runtime item scripts and data
    static void Shutdown();
  };
  
} // namespace mario
