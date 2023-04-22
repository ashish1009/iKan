//
//  serialise_entity.hpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#pragma once

#include "components.hpp"
#include <yaml-cpp/yaml.h>

namespace ikan {
  
  class EntitySerialiser {
  public:
    /// This function serialise the box 2d Collider component
    /// - Parameters:
    ///   - out: YAML out reference
    ///   - bcc: box collider component
    ///   - identifier: string identifier
    static void SerializeBoxCollider(YAML::Emitter& out, const Box2DColliderComponent& bcc, std::string identifier);
    /// This function deserialise the box 2d Collider component
    /// - Parameters:
    ///   - bcc: box collider component
    ///   - box_colloider_component: box collider data from YAML
    ///   - identifier: string identifier
    static void DeserializeBoxCollider(Box2DColliderComponent& bcc, const YAML::Node& box_colloider_component, std::string identifier);
    /// This function serialise the circle Collider component
    /// - Parameters:
    ///   - out: yamml out reference
    ///   - ccc: cirlce collider component
    ///   - identifier: string identifier
    static void SerializeCircleCollider(YAML::Emitter& out, const CircleColliiderComponent& ccc, std::string identifier);
    /// This function serialise the circle Collider component
    /// - Parameters:
    ///   - ccc: cirlce collider component
    ///   - circle_colloider_component: cirlce collider data from YAML
    ///   - identifier: string identifier
    static void DeserializeCircleCollider(CircleColliiderComponent& ccc, const YAML::Node& circle_colloider_component, std::string identifier);
    /// This function serialise entity with all component
    /// - Parameters:
    ///   - out: out Refenrence of YAML
    ///   - entity: entity
    static void SerialiseEntity(YAML::Emitter& out, Entity entity);
    /// This function deserialise the entity of all compoennt
    /// - Parameters:
    ///   - entity: entity data
    ///   - e: entity
    ///   - scene_: scene
    static void DeserislizeEntity(YAML::Node entity, Entity e, Scene* scene_);
    
    MAKE_PURE_STATIC(EntitySerialiser);
  };
  
} // namespace ikan
