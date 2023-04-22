//
//  ray_cast.hpp
//  ikan
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#include <box2d/b2_world.h>

namespace ikan  {

  class Entity;

  class RayCast2DInfo : public b2RayCastCallback {
  public:
    /// This constructore instantiate the Ray cast with game object
    /// - Parameter object: Game object Entity
    RayCast2DInfo(Entity* object);
    /// This function report the fuxture
    /// - Parameters:
    ///   - fixture: Fixture type
    ///   - hit_point: hit point
    ///   - normal: normal of the ray
    ///   - fraction: fraction of hit point
    float ReportFixture(b2Fixture* fixture, const b2Vec2& hit_point, const b2Vec2& normal, float fraction) override;
    /// This function checks the ray is Hitting ground (other Entity)
    bool OnGround() const;
    
  public:
    b2Fixture* fixture = nullptr;
    b2Vec2 hit_point;
    b2Vec2 normal;
    float fraction = 0.0f;
    bool hit = false;
    
    Entity* hit_object = nullptr;
    Entity* request_object = nullptr;
    UUID request_obj_id_;
  };
  
} // namespace ikan
