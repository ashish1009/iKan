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
    
    /// This function returns the texture of a sprite type
    /// - Parameter type: sprite type to return the texture
    static std::shared_ptr<Texture> GetTexture(SpriteType type);

    MAKE_PURE_STATIC(SpriteManager);
  };
  
} // namespace mario
