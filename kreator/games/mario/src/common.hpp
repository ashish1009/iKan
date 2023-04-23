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
  
#define MARIO_LOG(...) IK_TRACE("Mario", __VA_ARGS__);
  
  using namespace ikan;
  
  class MarioPrefab {
  public:
    /// This function add the quad component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: enity
    ///   - tex: texture to be added
    ///   - sprites: sprites vecotr
    static QuadComponent* AddQuad(Entity* entity, TextureRef tex, const std::vector<SubTextureRef>& sprites);
    /// This function add the Ridig Body component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - type: type of body
    static RigidBodyComponent* AddRigidBody(Entity* entity, RigidBodyComponent::RbBodyType type);
    /// This function add the Pill Box Collider component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - size: size of pill
    ///   - offset: offset of pill
    static PillBoxColliderComponent* AddPillBoxCollider(Entity* entity, const glm::vec2& size = glm::vec2(0.5f), const glm::vec2& offset = glm::vec2(0.0f));
    MAKE_PURE_STATIC(MarioPrefab);
  };
  
} // namespace
