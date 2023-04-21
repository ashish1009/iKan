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
    static void SerializeBoxCollider(YAML::Emitter& out, const Box2DColliderComponent& bcc, std::string identifier);
    static void DeserializeBoxCollider(Box2DColliderComponent& bcc, const YAML::Node& box_colloider_component, std::string identifier);
    static void SerializeCircleCollider(YAML::Emitter& out, const CircleColliiderComponent& ccc, std::string identifier);
    static void DeserializeCircleCollider(CircleColliiderComponent& ccc, const YAML::Node& circle_colloider_component, std::string identifier);
    static void SerialiseEntity(YAML::Emitter& out, Entity entity);
    static void DeserislizeEntity(YAML::Node entity, Entity e, Scene* scene_);
    
    MAKE_PURE_STATIC(EntitySerialiser);
  };
  
} // namespace ikan
