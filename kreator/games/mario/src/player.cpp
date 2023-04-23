//
//  player.cpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#include "player.hpp"

namespace mario {
  
  PlayerController* PlayerController::instance_ = nullptr;
  
  PlayerController::PlayerController() {
    MARIO_LOG("Creating Mario Player Controller");
    instance_ = this;
  }
  
  PlayerController::~PlayerController() {
    MARIO_LOG("Destroying Mario Player Controller");
  }
  
  void PlayerController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = &(GetComponent<RigidBodyComponent>());
    rbc_->SetGravityScale(0.0f);
  }
  
  void PlayerController::Update(Timestep ts) {
    
  }
  
  void PlayerController::RenderGui() {
    
  }
  
  void PlayerController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    
  }
  
  void PlayerController::Copy(void* script) {
    
  }
  
  void PlayerController::EventHandler(Event& event) {
    
  }
  

} // namespace mario
