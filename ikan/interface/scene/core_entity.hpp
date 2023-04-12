//
//  entity.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include "scene.hpp"

namespace ikan {
  
  /// enTT Wrapper class to get, add .. APIs
  struct Entity {
    /// This Constructor creates an Entity with handle and scene pointer
    /// - parameters:
    ///   - handle: handle of Entity. Scene (registry) takes care for this
    ///   - scene: Scene pointer to which this entity to be bound
    Entity(entt::entity handle = { entt::null }, Scene* scene = nullptr);
    /// This is default entity Destrcutor
    ~Entity();
    
    DEFINE_COPY_MOVE_CONSTRUCTORS(Entity);

    entt::entity entity_handle_{ entt::null };
    Scene* scene_ = nullptr;
    
  private:
    /// This function checks is Scene is valid
    bool IsValidScene() const;
  };
  
} // namespace ikan
