//
//  contact_listner.hpp
//  ikan
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace ikan {
  
  class ContactListner2D : public b2ContactListener {
  public:
    virtual ~ContactListner2D() = default;
    
    /// This Function starts the collision of two entites
    /// - Parameter contact: Contact of entity
    void BeginContact(b2Contact* contact) override;
    /// This Function Ends the collision of two entites
    /// - Parameter contact: Contact of entity
    void EndContact(b2Contact* contact) override;
    /// This Function Pre process the collision start
    /// - Parameters:
    ///   - contact: Contact of entity
    ///   - oldManifold: old manifold
    void PreSolve(b2Contact* contact, const b2Manifold* old_manifold) override;
    /// This Function Post process the collision end
    /// - Parameters:
    ///   - contact: Contact of entity
    ///   - impulse: Impulse of contact
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
  };
  
} // namespace ikan
