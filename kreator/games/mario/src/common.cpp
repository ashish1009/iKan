//
//  common.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "common.hpp"
#include "sprite_manager.hpp"

namespace mario {
  
  void MarioPrefab::AddQuadComponent(Entity *entity) {
    if (!entity->HasComponent<QuadComponent>()) {
//      auto& qc = entity->AddComponent<QuadComponent>();
//      qc.sprite.use = true;
//      qc.sprite.type = SpriteComponent::Type::Sprite;
//      qc.sprite.linear_edge = false;
//      qc.sprite.texture = SpriteManager::GetTexture(SpriteType::Player);
//      qc.sprite.sub_texture = nullptr; // SpriteManager::GetPlayerStateSprite(PlayerState::Small, PlayerAction::Idle).at(0);
    }
  }
  
} // namespace ikan
