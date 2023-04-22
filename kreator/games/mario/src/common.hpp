//
//  common.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#define TextureRef std::shared_ptr<Texture>
#define SubTextureRef std::shared_ptr<SubTexture>

namespace mario {
  
  using namespace ikan;
  
  class MarioPrefab {
  public:
    static void AddQuad(Entity* entity, TextureRef tex, const std::vector<SubTextureRef>& sprites);
    static void AddRigidBody(Entity* entity, RigidBodyComponent::RbBodyType type, bool fixed_rotation);
    static void AddPillBoxCollider(Entity* entity, float width = 0.5f, float height = 0.5f, const glm::vec2& offset = glm::vec2(0.0f));
    MAKE_PURE_STATIC(MarioPrefab);
  };
  
} // namespace
