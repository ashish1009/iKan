//
//  player.cpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#include "player.hpp"
#include "runtime_items.hpp"

namespace mario {
  
  PlayerController* PlayerController::instance_ = nullptr;
  
  bool PlayerController::IsPlayer(Entity* entity) {
    if (!Entity::IsValid(entity)) return false;
    return (entity->HasComponent<NativeScriptComponent>() and entity->GetComponent<NativeScriptComponent>().script.get() == instance_);
  }
  
  PlayerController::PlayerController() {
    MARIO_LOG("Player Controller Constructed");
    instance_ = this;
  }
  
  PlayerController::~PlayerController() {
    MARIO_LOG("Destroying Mario Player Controller");
  }
  
  void PlayerController::Create(Entity entity) {
    entity_ = entity;
    
    rbc_ = &(GetComponent<RigidBodyComponent>());
    rbc_->SetGravityScale(0.0f);
    rbc_->fixed_rotation = true;
    
    state_machine_ = std::make_shared<StateMachine>(&entity_);
    
    SetState(PlayerState::Small);
    MARIO_LOG("Creating Mario Player Controller");
  }
  
  void PlayerController::Update(Timestep ts) {
    // If player size is altered then upddate physics body
    if (rbc_->reset_fixture_) {
      const auto& pbc = entity_.GetComponent<PillBoxColliderComponent>();
      Scene::ResetPillBoxColliderFixture(entity_.GetComponent<TransformComponent>(), rbc_, pbc);
      rbc_->reset_fixture_ = false;
    }

    // If Player is powering Up then freez the player for powerup time
    if (powerup_time_ > 0) {
      powerup_time_ -= ts;
      rbc_->SetVelocity({0, 0});
      return;
    }
    
    // Check the player is landed on some A body which is ground
    CheckOnGround();
    
    // Poll the button for Running the jumping. If not jumping then land the player
    JumpAndLand(ts);
    
    // Poll the buttons for Running the player
    Run(ts);
    
    // If Hit the powerup block then Powerup the player
    if (power_up_)
      PowerUp(ts);
    
    // Add Velocity to player body
    velocity_.x += acceleration_.x * ts * 2.0f;
    velocity_.y += acceleration_.y * ts * 2.0f;
    
    velocity_.x = std::max(std::min(velocity_.x, terminal_velocity_.x), -terminal_velocity_.x);
    velocity_.y = std::max(std::min(velocity_.y, terminal_velocity_.y), -terminal_velocity_.y);
    
    rbc_->SetVelocity(velocity_);
    rbc_->SetAngularVelocity(0.0f);
  }
  
  void PlayerController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    // Immediatly NILL all forward velocity as player hits the obstacle of block
    if (std::abs(contact_normal.x) > 0.8f) {
      velocity_.x = 0.0f;
    }
    // Immediatly NILL all Vertical velocity as player hits the obstacle of block
    else if (contact_normal.y > 0.8f) {
      velocity_.y = 0;
      acceleration_.y = 0;
      jump_time_ = 0;
    }
  }
  
  void PlayerController::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(PlayerController::KeyPressed));
  }
  
  bool PlayerController::KeyPressed(KeyPressedEvent &key_event) {
    if (key_event.GetRepeatCount() > 0 or !state_machine_)
      return false;
    
    // Check Fire State and Spawn Fireball if button pressed
//    if (state_machine_->State() == PlayerState::Fire ) {
      if (key_event.GetKeyCode() == Key::X) {
        const auto& tc = entity_.GetComponent<TransformComponent>();
        float fireball_pos_x = (tc.Scale().x > 0) ? tc.Position().x + 1 : tc.Position().x - 1;
        float fireball_pos_y = tc.Position().y + 0.5;
        RuntimeItemManager::Spawn(Items::Fireball, entity_.scene_, {fireball_pos_x, fireball_pos_y}, NoScore);
      }
//    }
    
    return false;
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
      
      // Add Impulse to push player out of ground while changing size
      rbc_->ApplyImpulseToCenter({0, 1.0});
    }
    else if (state_machine_->State() == PlayerState::Fire) {
      // Do Nothing for Fire for now
    }
    entity_.GetComponent<TransformComponent>().UpdateScale(Y, height_);
    
    rbc_->reset_fixture_ = true;
  }
  
  void PlayerController::CheckOnGround() {
    float inner_player_width = width_ * 0.6f;
    float y_val = -(height_ / 2);
    y_val -= 0.02f;
    
    on_ground_ = entity_.scene_->CheckOnGround(&entity_, inner_player_width, y_val);
  }
  
  void PlayerController::JumpAndLand(Timestep ts) {
    // If
    // - Space is pressed and
    // - Either
    //    - Jump time is non zero (Player is in Air) or
    //    - Player is on Ground or
    //    - Ground Debound time is non zero (If a player is on some edge then before it falls we have ground_debounce_time_ left to jump again)
    if (Input::IsKeyPressed(Key::Space) and (jump_time_ > 0 or on_ground_ or ground_debounce_ > 0)) {
      // If
      // - Jump time is non zero (Player is in Air) and
      // - Either
      //    - Player is on Ground (Only edge) or
      //    - Ground Debound time is non zero (If a player is on some edge then before it falls we have ground_debounce_time_ left to jump again)
      if ((on_ground_ or ground_debounce_ > 0) and jump_time_ == 0) {
        jump_time_ = 55;
        velocity_.y = jump_impulse_;
      }
      // If Player is in Air
      else if (jump_time_ > 0) {
        jump_time_--;
        velocity_.y = (jump_time_ / 2.2f) * jumb_boost_;
      }
      // If Landed and Space Key is Pressed
      else {
        velocity_.y = 0.0f;
      }
      
      // Reset Ground debounce immediately after Space is pressed
      ground_debounce_ = 0.0f;
      
      // Player State is Jumping if player is on Air
      state_machine_->SetAction(PlayerAction::Jump);
    }
    else if (enemy_bounce_ > 0) {
      enemy_bounce_--;
      velocity_.y = ((enemy_bounce_ / 2.2f) * jumb_boost_);
    }
    else if (!on_ground_) {
      // If Player is in Air and Some jump time left. Then retard the Y Velocity and Make Jump time 0
      if (jump_time_ > 0) {
        velocity_.y *= 0.35f;
        jump_time_ = 0;
      }
      
      // If Space is not pressed yet and player is at Edge then wait for ground debounce time before player fell of the edge
      ground_debounce_ -= ts;

      // Free fall with scene gravity
      acceleration_.y = entity_.scene_->Get2DWorldGravity().y * free_fall_factor;
      
      // Player State is Jumping if player is on Air
      state_machine_->SetAction(PlayerAction::Jump);
    }
    else {
      // Set the Vertical velocity as 0
      velocity_.y = 0;
      acceleration_.y = 0;
      
      // Time to wait before player fell of the edge
      ground_debounce_ = ground_debounce_time_;
    }
  }
  
  void PlayerController::Run(Timestep ts) {
    auto& tc = entity_.GetComponent<TransformComponent>();
    if (Input::IsKeyPressed(Key::Left)) {
      // Change the direction
      tc.UpdateScale(X, -width_);
      
      // Update the direction and value of X axis acceleration
      acceleration_.x = -walk_speed_;
      
      // If already have different direction motion then slow it down. Switching Side
      if (velocity_.x > 0) {
        velocity_.x -= slow_down_force_;
        
        if (on_ground_)
          state_machine_->SetAction(PlayerAction::SwitchSide);
      }
      else {
        if (on_ground_)
          state_machine_->SetAction(PlayerAction::Run);
      }
    }
    else if (Input::IsKeyPressed(Key::Right)) {
      // Change the direction
      tc.UpdateScale(X, width_);
      
      // Update the direction and value of X axis acceleration
      acceleration_.x = walk_speed_;
      
      // If already have different direction motion then slow it down. Switching Side
      if (velocity_.x < 0) {
        velocity_.x += slow_down_force_;
        if (on_ground_)
          state_machine_->SetAction(PlayerAction::SwitchSide);
      }
      else {
        if (on_ground_)
          state_machine_->SetAction(PlayerAction::Run);
      }
    }
    else {
      // Friction Stop
      // Note : Not considering Object friction as player is not in contact with ground. Player is landing by tracking ray tracing
      acceleration_.x = 0;
      if (velocity_.x > 0) {
        velocity_.x = std::max(0.0f, velocity_.x - slow_down_force_);
      }
      else if (velocity_.x < 0) {
        velocity_.x = std::min(0.0f, velocity_.x + slow_down_force_);
      }
      
      if (on_ground_) {
        // If Player stopped then Set state to Idle
        if (velocity_.x == 0) {
          state_machine_->SetAction(PlayerAction::Idle);
        }
        else if (velocity_.x !=0) {
          state_machine_->SetAction(PlayerAction::Run);
        }
      }
    }
  }
  
  void PlayerController::PowerUp(Timestep ts) {
    if (state_machine_->State() == PlayerState::Small or state_machine_->State() == PlayerState::Big)
      state_machine_->SetAction(PlayerAction::PowerUp);
    
    powerup_time_ = 0.4; // Seconds
    power_up_ = false;

    if (IsSmall()) {
      SetState(PlayerState::Big);
      
      // Boost player run and jump speed
      jumb_boost_ *= jump_boost_factor_;
      walk_speed_ *= jump_boost_factor_;
    }
    else {
      SetState(PlayerState::Fire);
    }
  }
  
  void PlayerController::Copy(void* script) {
    if (!script) return;
    PlayerController* player_script = reinterpret_cast<PlayerController*>(script);
    IK_ASSERT(player_script);
    
    on_ground_ = player_script->on_ground_;
    
    width_ = player_script->width_;
    height_ = player_script->height_;
    
    walk_speed_ = player_script->walk_speed_;
    velocity_ = player_script->velocity_;
    acceleration_ = player_script->acceleration_;
    
    enemy_bounce_ = player_script->enemy_bounce_;
    jump_time_ = player_script->jump_time_;
    jumb_boost_ = player_script->jumb_boost_;
    ground_debounce_ = player_script->ground_debounce_;
    ground_debounce_time_ = player_script->ground_debounce_time_;
    
    score_ = player_script->score_;
    coins_ = player_script->coins_;
    
    power_up_ = player_script->power_up_;
    powerup_time_ = player_script->powerup_time_;
  }
  
  void PlayerController::RenderGui() {
    if (state_machine_) {
      ImGui::Text(" State             | %s", state_machine_->StateString().c_str());
      ImGui::Text(" Action            | %s", state_machine_->ActionString().c_str());
    }
    ImGui::Text(" On Ground         | %s", on_ground_ ? "True" : "False");
    ImGui::Separator();
    ImGui::Text(" Size              | %f x %f", width_, height_);
  }
  
} // namespace mario
