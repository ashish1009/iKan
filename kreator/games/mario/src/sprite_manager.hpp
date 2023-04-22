//
//  sprite_manager.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#include "common.hpp"

namespace mario {
  
  using namespace ikan;
  
  enum class PlayerAction;
  enum class PlayerState;

  enum class SpriteType {
    Player, Items, Enemy, Tile
  };
  
  class SpriteManager {
  public:
    /// This function initialise the sprite manager with all the sprite texture data
    static void Init();
    /// This function shutdowns the sprite manager with all the sprite texture data
    static void Shutdown();
    
    /// This function returns the texture of a sprite type
    /// - Parameter type: sprite type to return the texture
    static TextureRef GetTexture(SpriteType type);
    static const std::vector<SubTextureRef>& GetPlayerSprite(PlayerState state, PlayerAction action);

    MAKE_PURE_STATIC(SpriteManager);
  };
  
} // namespace mario
