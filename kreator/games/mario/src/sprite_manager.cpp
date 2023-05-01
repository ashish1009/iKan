//
//  sprite_manager.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "sprite_manager.hpp"
#include "player.hpp"
#include "runtime_items.hpp"

namespace mario {

  struct SpriteData {
    // Stores the Texture of all Sprite type
    std::unordered_map<SpriteType, TextureRef> texture_map;
    // Stores the Subtextures of Player for each Action and State Combination
    std::unordered_map<PlayerState, std::unordered_map<PlayerAction, std::vector<SubTextureRef>>> player_subtextures_map;
    // Stores the Subtextures of Player for each Action and State Combination
    std::unordered_map<PlayerAction, std::vector<SubTextureRef>> invinc_player_subtextures_map;
    // Stores the Runtime Item Subtextures
    std::unordered_map<Items, std::vector<SubTextureRef>> item_subtextures_map;
  };
  static std::shared_ptr<SpriteData> data_;

  void SpriteManager::Init() {
    data_ = std::make_shared<SpriteData>();
    
    // Store Textures
    data_->texture_map[SpriteType::Player] = Renderer::GetTexture(DM::ClientAsset("textures/player.png"), false);
    data_->texture_map[SpriteType::Items] = Renderer::GetTexture(DM::ClientAsset("textures/item.png"), false);
    data_->texture_map[SpriteType::Enemy] = Renderer::GetTexture(DM::ClientAsset("textures/enemy.png"), false);
    data_->texture_map[SpriteType::Tile] = Renderer::GetTexture(DM::ClientAsset("textures/tile.png"), false);
    
    // Player
    const auto& player_sprite = GetTexture(SpriteType::Player);
    
    // Small Player
    auto& small_player_map = data_->player_subtextures_map[PlayerState::Small];
    small_player_map[PlayerAction::Idle].push_back(SubTexture::CreateFromCoords(player_sprite, {6.0f, 30.0f}));
    small_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {0.0f, 30.0f}));
    small_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {1.0f, 30.0f}));
    small_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {2.0f, 30.0f}));
    small_player_map[PlayerAction::SwitchSide].push_back(SubTexture::CreateFromCoords(player_sprite, {3.0f, 30.0f}));
    small_player_map[PlayerAction::Jump].push_back(SubTexture::CreateFromCoords(player_sprite, {4.0f, 30.0f}));
    small_player_map[PlayerAction::Die].push_back(SubTexture::CreateFromCoords(player_sprite, {5.0f, 30.0f}));
    small_player_map[PlayerAction::PowerUp].push_back(SubTexture::CreateFromCoords(player_sprite, {6.0f, 30.0f}));
    small_player_map[PlayerAction::PowerUp].push_back(SubTexture::CreateFromCoords(player_sprite, {15.0f, 31.0f}, {1.0f, 2.0f}));
    
    // Big Player
    auto& big_player_map = data_->player_subtextures_map[PlayerState::Big];
    big_player_map[PlayerAction::Idle].push_back(SubTexture::CreateFromCoords(player_sprite, {6.0f, 31.0f}, {1.0f, 2.0f}));
    big_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {0.0f, 31.0f}, {1.0f, 2.0f}));
    big_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {1.0f, 31.0f}, {1.0f, 2.0f}));
    big_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {2.0f, 31.0f}, {1.0f, 2.0f}));
    big_player_map[PlayerAction::SwitchSide].push_back(SubTexture::CreateFromCoords(player_sprite, {3.0f, 31.0f}, {1.0f, 2.0f}));
    big_player_map[PlayerAction::Jump].push_back(SubTexture::CreateFromCoords(player_sprite, {4.0f, 31.0f}, {1.0f, 2.0f}));
    
    // Fire Player
    auto& fire_player_map = data_->player_subtextures_map[PlayerState::Fire];
    fire_player_map[PlayerAction::Idle].push_back(SubTexture::CreateFromCoords(player_sprite, {6.0f, 28.0f}, {1.0f, 2.0f}));
    fire_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {0.0f, 28.0f}, {1.0f, 2.0f}));
    fire_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {1.0f, 28.0f}, {1.0f, 2.0f}));
    fire_player_map[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {2.0f, 28.0f}, {1.0f, 2.0f}));
    fire_player_map[PlayerAction::SwitchSide].push_back(SubTexture::CreateFromCoords(player_sprite, {3.0f, 28.0f}, {1.0f, 2.0f}));
    fire_player_map[PlayerAction::Jump].push_back(SubTexture::CreateFromCoords(player_sprite, {4.0f, 28.0f}, {1.0f, 2.0f}));
    
    auto& invinc_player_subtexture = data_->invinc_player_subtextures_map;
    for (int32_t i = 0; i < 5; i++) {
      invinc_player_subtexture[PlayerAction::Idle].push_back(SubTexture::CreateFromCoords(player_sprite, {6.0f, 31.0f - (3 * i)}, {1.0f, 2.0f}));
      invinc_player_subtexture[PlayerAction::Run].push_back(SubTexture::CreateFromCoords(player_sprite, {1.0f, 31.0f - (3 * i)}, {1.0f, 2.0f}));
      invinc_player_subtexture[PlayerAction::SwitchSide].push_back(SubTexture::CreateFromCoords(player_sprite, {3.0f, 31.0f - (3 * i)}, {1.0f, 2.0f}));
      invinc_player_subtexture[PlayerAction::Jump].push_back(SubTexture::CreateFromCoords(player_sprite, {3.0f, 31.0f - (3 * i)}, {1.0f, 2.0f}));
    }

    // Items
    auto& item_sprite = data_->texture_map[SpriteType::Items];
    data_->item_subtextures_map[Items::Coin].push_back(SubTexture::CreateFromCoords(item_sprite, {0.0f, 14.0f}));
    data_->item_subtextures_map[Items::Mushroom].push_back(SubTexture::CreateFromCoords(item_sprite, {0.0f, 19.0f}));
    data_->item_subtextures_map[Items::Flower].push_back(SubTexture::CreateFromCoords(item_sprite, {0.0f, 18.0f}));
    data_->item_subtextures_map[Items::Flower].push_back(SubTexture::CreateFromCoords(item_sprite, {1.0f, 18.0f}));
    data_->item_subtextures_map[Items::Flower].push_back(SubTexture::CreateFromCoords(item_sprite, {2.0f, 18.0f}));
    data_->item_subtextures_map[Items::Flower].push_back(SubTexture::CreateFromCoords(item_sprite, {3.0f, 18.0f}));
    data_->item_subtextures_map[Items::Fireball].push_back(SubTexture::CreateFromCoords(item_sprite, {7.0f, 10.0f}));
    data_->item_subtextures_map[Items::BigFireball].push_back(SubTexture::CreateFromCoords(item_sprite, {7.0f, 9.0f}));
    data_->item_subtextures_map[Items::InactiveBlock].push_back(SubTexture::CreateFromCoords(item_sprite, {3.0f, 8.0f}));

    MARIO_LOG("Initialised the Sprite Manager");
  }
  
  void SpriteManager::Shutdown() {
    data_->texture_map.clear();
    data_.reset();
    
    MARIO_LOG("Destroyed the Sprite Manager");
  }
  
  TextureRef SpriteManager::GetTexture(SpriteType type) {
    if (data_ and data_->texture_map.find(type) != data_->texture_map.end())
      return data_->texture_map.at(type);
    return nullptr;
  }

  const std::vector<SubTextureRef>& SpriteManager::GetPlayerSprite(PlayerState state, PlayerAction action, PlayerAction prev_action) {
    // For Powerup while Big we need to animate the Player to all color based on the prev action
    if (action == PlayerAction::PowerUp and state == PlayerState::Big) {
      const auto& invince_player_map = data_->invinc_player_subtextures_map;
      IK_ASSERT(invince_player_map.find(prev_action) != invince_player_map.end());
      
      return invince_player_map.at(prev_action);
    }

    IK_ASSERT(data_ or data_->player_subtextures_map.find(state) != data_->player_subtextures_map.end());
    
    const auto& player_map = data_->player_subtextures_map.at(state);
    IK_ASSERT(player_map.find(action) != player_map.end());
    
    return player_map.at(action);
  }

  std::vector<SubTextureRef> SpriteManager::GetItemSprite(Items item) {
    IK_ASSERT(data_ or data_->item_subtextures_map.find(item) != data_->item_subtextures_map.end());
    return data_->item_subtextures_map.at(item);
  }

} // namespace mario
