//
//  player_state_machine.cpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#include "player_state_machine.hpp"
#include "sprite_manager.hpp"

namespace mario {
  
  StateMachine::StateMachine(Entity* entity) {
    player_entity_ = entity;
    SetState(PlayerState::Small);
    SetAction(PlayerAction::Idle);
  }
  
  void StateMachine::SetAction(PlayerAction new_action) {
    player_prev_action_ = player_action_ ;
    player_action_ = new_action;
    
    auto& qc = player_entity_->GetComponent<QuadComponent>();
    qc.sprite.sprite_images = SpriteManager::GetPlayerSprite(player_state_, player_action_);
  }
  
  void StateMachine::SetState(PlayerState new_state) {
    player_prev_state_ = player_state_;
    player_state_ = new_state;
  }

  std::string StateMachine::StateString() const {
    switch (player_state_) {
      case PlayerState::Invalid:      IK_ASSERT(false);
      case PlayerState::Small:        return "Small";
      case PlayerState::Big:          return "Big";
      case PlayerState::Fire:         return "Fire";
    }
  }
  
  std::string StateMachine::ActionString() const {
    switch (player_action_) {
      case PlayerAction::Invalid:     IK_ASSERT(false);
      case PlayerAction::Idle:        return "Idle";
      case PlayerAction::Run:         return "Running";
      case PlayerAction::SwitchSide:  return "Switch Side";
      case PlayerAction::Jump:        return "Jumping";
      case PlayerAction::Die:         return "Dying";
      case PlayerAction::PowerUp:     return "PowerUp";
    }
  }
  
  PlayerState StateMachine::State() const { IK_ASSERT(player_state_ != PlayerState::Invalid); return player_state_; }
  PlayerState StateMachine::PrevState() const { IK_ASSERT(player_state_ != PlayerState::Invalid); return player_prev_state_; }
  PlayerAction StateMachine::Action() const { IK_ASSERT(player_action_ != PlayerAction::Invalid); return player_action_; }
  PlayerAction StateMachine::PrevAction() const { IK_ASSERT(player_action_ != PlayerAction::Invalid); return player_prev_action_; }
  
} // namespace mario
