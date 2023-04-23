//
//  player.hpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#pragma once

#include "common.hpp"

namespace mario {
  
  using namespace ikan;
  
  enum class PlayerAction {
    Invalid, Idle, Run, SwitchSide, Jump, Die, PowerUp
  };
  
  enum class PlayerState {
    Invalid, Small, Big, Fire
  };
  
  class PlayerController : public ScriptableEntity {
  public:
    PlayerController();
    ~PlayerController();
    
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void RenderGui() override;
    void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    void EventHandler(Event& event) override;

    static PlayerController* Get() { return instance_; }

  private:
    static PlayerController* instance_;
  };
  
} // namespace mario
