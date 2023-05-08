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
    virtual ~PlayerController();

    // Documentation @ScriptableEntity
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void RenderGui() override;
    void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    void EventHandler(Event& event) override;
    
    /// This function increment the coin count
    void IncCoin() { coins_++; }
    /// This function adds the score of plyaer
    /// - Parameter score: score
    void AddScore(uint32_t score) { score_ += score; }
    
    /// This function returns the score of player
    uint32_t GetScore() const { return score_; }
    /// This function returns the coin count of player
    uint32_t GetCoins() const { return coins_; }
    
    /// This function returns if player is small
    bool IsSmall() const { return state_machine_->State() == PlayerState::Small; }
    /// This function update the powerup flag to true
    void SetPowerup() { power_up_ = true; };
    /// This function set the enemy debounce value
    void SetEnemyBounce() { enemy_bounce_ = 18; }
    /// This function checks is player direction is right
    bool IsRight() const { return entity_.GetComponent<TransformComponent>().Scale().x > 0; }
    
    /// This functin checks the entity is player
    /// - Parameter entity: entity
    static bool IsPlayer(Entity* entity);
    /// This function return the current instance of player
    static PlayerController* Get() { return instance_; }

  private:
    /// This function dispatched in event dispatcher and trigger when Key is pressed
    /// - Parameter key_event: Key event instacnce
    bool KeyPressed(KeyPressedEvent& event);
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
    /// This function powers up the player on hitting the powerup Item
    /// - Parameter ts: time step
    void PowerUp(Timestep ts);

    // Constants
    static constexpr float free_fall_factor = 2.7f;
    static constexpr float slow_down_force_ = 0.08f;
    static constexpr float jump_impulse_ = 10.0f;
    static constexpr float jump_boost_factor_ = 1.5f;
    static constexpr glm::vec2 terminal_velocity_ = {8.1f, 18.1f};

    // Member Variables
    bool on_ground_ = false;

    // Size of player
    float width_ = 1.0f, height_ = 1.0f;
    
    // Player Movement data
    float walk_speed_ = 4.0f;
    glm::vec2 acceleration_;
    glm::vec2 velocity_;
    
    // Jump data
    int32_t enemy_bounce_ = 0;
    int32_t jump_time_ = 0;
    float jumb_boost_ = 1.0f;
    float ground_debounce_ = 0.0f; // Seconds
    float ground_debounce_time_ = 0.1f; // Seconds
    
    // Powerup data
    bool power_up_ = false;
    float powerup_time_ = 0.0f;

    uint32_t score_ = 0;
    uint32_t coins_ = 0;
    
    std::shared_ptr<StateMachine> state_machine_;
    static PlayerController* instance_;
  };
  
} // namespace mario
