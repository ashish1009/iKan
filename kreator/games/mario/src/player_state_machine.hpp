//
//  player_state_machine.hpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class PlayerAction {
    Invalid, Idle, Run, SwitchSide, Jump, Die, PowerUp
  };
  
  enum class PlayerState {
    Invalid, Small, Big, Fire
  };
  
  class StateMachine {
  public:
    /// Constructor of player state machine
    /// - Parameter entity: player entity
    StateMachine(Entity* entity);
    
    /// This function return the current state of plyaer
    PlayerState State() const;

    /// This function returns the previous state of player
    PlayerState PrevState() const;
    /// This function returns the current action of player
    PlayerAction Action() const;
    /// This function returns the previous action of player
    PlayerAction PrevAction() const;
    
    /// This function Set the new action of player
    /// - Parameter new_action: new action
    void SetAction(PlayerAction new_action);
    /// This function set the new state of player
    /// - Parameter new_state: new state
    void SetState(PlayerState new_state);
    
    /// This function returns the player state in string
    std::string StateString() const;
    /// This function returns the player action in string
    std::string ActionString() const;

  private:
    Entity* player_entity_;
    
    PlayerState player_prev_state_ = PlayerState::Invalid;
    PlayerState player_state_ = PlayerState::Small;
    
    PlayerAction player_action_ = PlayerAction::Idle;
    PlayerAction player_prev_action_ = PlayerAction::Invalid;
  };
  
} // namespace mario
