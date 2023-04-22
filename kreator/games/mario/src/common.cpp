//
//  common.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "common.hpp"

#define GET_COMPONENT(comp) entity->HasComponent<comp>() ? &(entity->GetComponent<comp>()) : &(entity->AddComponent<comp>());

namespace mario {
  
  void MarioPrefab::AddQuad(Entity *entity, TextureRef tex, const std::vector<SubTextureRef>& sprites) {
    QuadComponent* qc = GET_COMPONENT(QuadComponent);
    
    qc->sprite.use = true;
    qc->sprite.use_sub_texture = true;
    qc->sprite.linear_edge = false;
    
    qc->sprite.texture.reset();
    qc->sprite.texture = tex;
    
    qc->sprite.ClearSprites();
    for (const auto& sprite : sprites) {
      if (sprite) {
        qc->sprite.sprite_images.emplace_back(sprite);
      }
    }
  }
  
  void MarioPrefab::AddRigidBody(Entity *entity, RigidBodyComponent::RbBodyType type, bool fixed_rotation) {
    RigidBodyComponent* rbc = GET_COMPONENT(RigidBodyComponent);
    rbc->type = type;
    rbc->fixed_rotation = fixed_rotation;
  }
  
  void MarioPrefab::AddPillBoxCollider(Entity *entity, float width, float height, const glm::vec2& offset) {
    PillBoxColliderComponent* pbc = GET_COMPONENT(PillBoxColliderComponent);
    pbc->width = width;
    pbc->height = height;
    pbc->offset = offset;
    pbc->RecalculateColliders();
  }
  
} // namespace ikan
