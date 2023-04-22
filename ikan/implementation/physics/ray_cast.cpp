//
//  ray_cast.cpp
//  ikan
//
//  Created by Ashish . on 22/04/23.
//

#include "ray_cast.hpp"
#include "scene/core_entity.hpp"
#include "scene/components.hpp"
#include <box2d/b2_fixture.h>

namespace ikan {
  
  RayCast2DInfo::RayCast2DInfo(Entity* object) {
    request_object = object;
  }

  float RayCast2DInfo::ReportFixture(b2Fixture *fixture, const b2Vec2 &hit_point, const b2Vec2 &normal, float fraction) {
    if (!Entity::IsValid((Entity*)(fixture->GetUserData().pointer)))
      return 1;
    
    if (((Entity*)(fixture->GetUserData().pointer))->GetComponent<IDComponent>().id ==
        request_object->GetComponent<IDComponent>().id) {
      return 1;
    }
    
    this->fixture = fixture;
    this->hit_point = hit_point;
    this->normal = normal;
    this->hit = true;
    this->fraction = fraction;
    this->hit_object = (Entity*)fixture->GetUserData().pointer;
    
    return fraction;
  }
  
  bool RayCast2DInfo::OnGround() const {
    return hit and hit_object and hit_object->HasComponent<RigidBodyComponent>() and hit_object->GetComponent<RigidBodyComponent>().is_ground;
  }
  
} // namespace ikan
