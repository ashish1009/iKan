//
//  common.cpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#include "common.hpp"

namespace mario {
  
  void MarioPrefab::AddQuadComponent(Entity *entity, TextureRef tex, const std::vector<SubTextureRef>& sprites) {
    QuadComponent* qc;
    if (!entity->HasComponent<QuadComponent>()) {
      qc = &(entity->AddComponent<QuadComponent>());
    } else {
      qc = &(entity->GetComponent<QuadComponent>());
    }
    
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
  
} // namespace ikan
