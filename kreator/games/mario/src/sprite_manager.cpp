//
//  sprite_manager.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "sprite_manager.hpp"

namespace mario {

  struct SpriteData {
    std::unordered_map<SpriteType, std::shared_ptr<Texture>> texture_map;
  };
  static std::shared_ptr<SpriteData> data_;

  void SpriteManager::Init() {
    data_ = std::make_shared<SpriteData>();
    
    // Store Textures
    data_->texture_map[SpriteType::Player] = Renderer::GetTexture(DM::ClientAsset("textures/player.png"), false);
    data_->texture_map[SpriteType::Items] = Renderer::GetTexture(DM::ClientAsset("textures/item.png"), false);
    data_->texture_map[SpriteType::Enemy] = Renderer::GetTexture(DM::ClientAsset("textures/enemy.png"), false);
    data_->texture_map[SpriteType::Tile] = Renderer::GetTexture(DM::ClientAsset("textures/tile.png"), false);
  }
  
  void SpriteManager::Shutdown() {
    data_->texture_map.clear();
    data_.reset();
  }
  
  std::shared_ptr<Texture> SpriteManager::GetTexture(SpriteType type) {
    if (data_ and data_->texture_map.find(type) != data_->texture_map.end())
      return data_->texture_map.at(type);
    return nullptr;
  }

} // namespace mario
