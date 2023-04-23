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
    
    SetState(PlayerState::Small);
  }
  
  void PlayerController::Update(Timestep ts) {
    
  }
  
  void PlayerController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    
  }
  
  void PlayerController::EventHandler(Event& event) {
    
  }

  void PlayerController::SetState(PlayerState new_state) {
    state_machine_->SetState(new_state);
    
    auto& pbc = entity_.GetComponent<PillBoxColliderComponent>();
    if (state_machine_->State() == PlayerState::Small) {
      width_ = 1.0f;
      height_ = 1.0f;
      pbc.SetSize({0.4f, height_ / 2.0f});
    }
    else if (state_machine_->State() == PlayerState::Big) {
      width_ = 1.0f;
      height_ = 2.0f;
      pbc.SetSize({0.5f, height_ / 2.0f});
    }
    else if (state_machine_->State() == PlayerState::Fire) {
      // Do Nothing for Fire for now
    }
    entity_.GetComponent<TransformComponent>().UpdateScale(Y, height_);
  }
  
  void PlayerController::Copy(void* script) {
    if (!script) return;
    PlayerController* player_script = reinterpret_cast<PlayerController*>(script);
    IK_ASSERT(player_script);
    
    width_ = player_script->width_;
    height_ = player_script->height_;
  }
  
  void PlayerController::RenderGui() {
    if (state_machine_) {
      ImGui::Text(" State             | %s", state_machine_->StateString().c_str());
      ImGui::Text(" Action            | %s", state_machine_->ActionString().c_str());
    }
    ImGui::Text(" Size              | %f x %f", width_, height_);
  }

} // namespace mario
