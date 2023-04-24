//
//  player.hpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#pragma once

#include "common.hpp"
#include "player_state_machine.hpp"

namespace mario {
  
  using namespace ikan;
    
  class PlayerController : public ScriptableEntity {
  public:
    /// Default Constructor of Player controller
    PlayerController();
    /// Default Desructor of Player controller
    ~PlayerController();

    // Documentation @ScriptableEntity
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void RenderGui() override;
    void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    void EventHandler(Event& event) override;
    
    /// This function return the current instance of player
    static PlayerController* Get() { return instance_; }

  private:
    /// This function wraps the state machine change state, also update the size of plyaer entity based on state
    /// - Parameter new_state: new state
    void SetState(PlayerState new_state);
    /// This function checks the player hits the ground rigid body
    void CheckOnGround();
    /// This function jumps the player on polling the space button. Also check the ground debounce time and enemy bounce
    /// - Parameter ts: Time step of each frame
    void JumpAndLand(Timestep ts);
    /// This function runs the player on polling the Left or right button
    /// - Parameter ts: Time step of each frame
    void Run(Timestep ts);
    
    // Constants
    static constexpr float free_fall_factor = 2.7f;
    static constexpr float slow_down_force_ = 0.08f;

    // Member Variables
    bool on_ground_ = false;

    // Size of player
    float width_ = 1.0f, height_ = 1.0f;
    
    // Player Movement data
    float walk_speed_ = 4.0f;
    glm::vec2 acceleration_;
    glm::vec2 velocity_;
    glm::vec2 terminal_velocity_ = {8.1f, 18.1f};

    std::shared_ptr<StateMachine> state_machine_;
    static PlayerController* instance_;
  };
  
} // namespace mario
