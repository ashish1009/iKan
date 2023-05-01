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
  enum class Items;

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
    /// This function returns all the subtextures in vector requred for an action for a player state
    /// - Parameters:
    ///   - state: player state
    ///   - action: player action
    static const std::vector<SubTextureRef>& GetPlayerSprite(PlayerState state, PlayerAction action, PlayerAction prev_action);
    /// This Function returns the Runtime Item Subtexture
    /// - Parameter type: Item Type
    static std::vector<SubTextureRef> GetItemSprite(Items type);

    MAKE_PURE_STATIC(SpriteManager);
  };
  
  using SM = SpriteManager;
  
} // namespace mario
