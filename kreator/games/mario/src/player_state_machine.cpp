//
//  player_state_machine.cpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#include "player_state_machine.hpp"

namespace mario {
  
  StateMachine::StateMachine(Entity* entity) {
    player_entity_ = entity;
    SetState(PlayerState::Small);
    SetAction(PlayerAction::Idle);
  }
  
  void StateMachine::SetAction(PlayerAction new_action) {
    player_prev_action_ = player_action_ ;
    player_action_ = new_action;
  }
  
  void StateMachine::SetState(PlayerState new_state) {
    player_prev_state_ = player_state_;
    player_state_ = new_state;
  }

  PlayerState StateMachine::State() const { IK_ASSERT(player_state_ != PlayerState::Invalid); return player_state_; }
  PlayerState StateMachine::PrevState() const { IK_ASSERT(player_state_ != PlayerState::Invalid); return player_prev_state_; }
  PlayerAction StateMachine::Action() const { IK_ASSERT(player_action_ != PlayerAction::Invalid); return player_action_; }
  PlayerAction StateMachine::PrevAction() const { IK_ASSERT(player_action_ != PlayerAction::Invalid); return player_prev_action_; }
  
} // namespace mario
