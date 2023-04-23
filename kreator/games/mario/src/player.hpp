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
    std::shared_ptr<StateMachine> state_machine_;
    static PlayerController* instance_;
  };
  
} // namespace mario
