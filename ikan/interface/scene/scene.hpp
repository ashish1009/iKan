//
//  scene.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include <entt.hpp>

namespace ikan {
  
  class Scene {
  public:
    enum State : uint8_t {
      Play, Edit
    };
    
    /// Default constructor of Scene
    Scene();

    /// This function sets the Scene as play mode
    void PlayScene();
    /// This function sets the Scene as edit mode
    void EditScene();

    /// This function returns the state of scene
    State GetState() const { return state_; }
    /// This finction return is scene is in edit state
    bool IsEditing() const { return state_ == Edit; }

    /// This function create new scene copy the scene data from argument
    /// - Parameter other: copy scene
    static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);

  private:
    entt::registry registry_;
    State state_ = State::Edit;
  };
  
} // namespace ikan
