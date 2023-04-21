//
//  scriptable_entity.hpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#pragma once

#include "scene/core_entity.hpp"
#include <box2d/b2_contact.h>

namespace ikan {
  
  class ScriptableEntity {
  public:
    template <typename... Args>
    ScriptableEntity(Args... args) {}
    virtual ~ScriptableEntity() {}
    
    template<typename T> T& GetComponent() { return entity_.GetComponent<T>(); }
    template<typename T> bool HasComponent() { return entity_.HasComponent<T>(); }
    
    virtual void EventHandler(Event& event) {}
    virtual void RenderGui() {}
    virtual void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void EndCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void PostSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void Copy(void* script) {}
    
  protected:
    virtual void Create(Entity entity) { entity_ = entity; }
    virtual void Destroy() {}
    virtual void Update(Timestep ts) {}
    
  protected:
    Entity entity_;
    Scene* scene_;
    friend class EnttScene;
  };
  
} // namespace ikan
