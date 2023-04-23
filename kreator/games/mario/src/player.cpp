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
    
    state_machine_ = std::make_shared<StateMachine>(&entity_);
  }
  
  void PlayerController::Update(Timestep ts) {
    
  }
  
  void PlayerController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    
  }
  
  void PlayerController::EventHandler(Event& event) {
    
  }

  void PlayerController::Copy(void* script) {
    
  }
  
  void PlayerController::RenderGui() {
    if (state_machine_) {
      ImGui::Text(" State             | %s", state_machine_->StateString().c_str());
      ImGui::Text(" Action            | %s", state_machine_->ActionString().c_str());
    }
  }

} // namespace mario
