//
//  common.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "common.hpp"

#define GET_COMPONENT(comp) entity->HasComponent<comp>() ? &(entity->GetComponent<comp>()) : &(entity->AddComponent<comp>());

namespace mario {
  
  QuadComponent* MarioPrefab::AddQuad(Entity *entity, TextureRef tex, const std::vector<SubTextureRef>& sprites) {
    QuadComponent* qc = GET_COMPONENT(QuadComponent);
    
    qc->sprite.use = true;
    qc->sprite.use_sub_texture = true;
    qc->sprite.linear_edge = false;
    
    qc->sprite.ClearTextures();
    qc->sprite.texture.push_back(tex);
    
    qc->sprite.ClearSprites();
    for (const auto& sprite : sprites) {
      if (sprite) {
        qc->sprite.sprite_images.emplace_back(sprite);
      }
    }
    return qc;
  }
  
  RigidBodyComponent* MarioPrefab::AddRigidBody(Entity *entity, RigidBodyComponent::RbBodyType type) {
    RigidBodyComponent* rbc = GET_COMPONENT(RigidBodyComponent);
    rbc->type = type;
    return rbc;
  }
  
  PillBoxColliderComponent* MarioPrefab::AddPillBoxCollider(Entity *entity, const glm::vec2& size, const glm::vec2& offset) {
    PillBoxColliderComponent* pbc = GET_COMPONENT(PillBoxColliderComponent);
    pbc->width = size.x;
    pbc->height = size.y;
    pbc->offset = offset;
    pbc->RecalculateColliders();
    return pbc;
  }

  CircleColliiderComponent* MarioPrefab::AddCircleCollider(Entity *entity, float radius, const glm::vec2& offset) {
    CircleColliiderComponent* ccc = GET_COMPONENT(CircleColliiderComponent);
    ccc->radius = radius;
    ccc->offset = offset;
    return ccc;
  }

  TextComponent* MarioPrefab::AddText(Entity *entity, const std::string &text, const glm::vec4& color) {
  TextComponent* tc = GET_COMPONENT(TextComponent);
    tc->text = text;
    tc->color = color;
    return tc;
  }

} // namespace ikan
