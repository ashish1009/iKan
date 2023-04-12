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
    
    /// This Constructor creates the instance of Scene.
    /// - Parameter file_path: optional file path if we want to create a pre saved scene
    Scene(const std::string& file_path = "Unsaved_Scene");

    /// This function sets the Scene as play mode
    void PlayScene();
    /// This function sets the Scene as edit mode
    void EditScene();

    /// This function update the scene path
    /// - Parameter file_path: file path
    void SetFilePath(const std::string& file_path);
    
    /// This function returns the state of scene
    State GetState() const { return state_; }
    /// This finction return is scene is in edit state
    bool IsEditing() const { return state_ == Edit; }
    /// This finction return Name of scene
    const std::string& GetName() const { return name_; }
    /// This finction return File path of scene
    const std::string& GetFilePath() const { return file_path_; }

    /// This function create new scene copy the scene data from argument
    /// - Parameter other: copy scene
    static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);

  private:
    std::string file_path_ = "Unsaved_Scene", name_ = "Unsaved_Scene";
    entt::registry registry_;
    State state_ = State::Edit;
    
    friend class SceneSerializer;
    friend class ScenePanelManager;
  };
  
} // namespace ikan
