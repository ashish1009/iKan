//
//  sprite_manager.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
    
  enum class SpriteType {
    Player, Items, Enemy, Tile
  };
  
  class SpriteManager {
  public:
    /// This function initialise the sprite manager with all the sprite texture data
    static void Init();
    /// This function shutdowns the sprite manager with all the sprite texture data
    static void Shutdown();
    
    MAKE_PURE_STATIC(SpriteManager);
  };
  
} // namespace mario
