//
//  scene.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "scene.hpp"

namespace ikan {
  
  std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other) {
    std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();
    return new_scene;
  }

  Scene::Scene() {
    IK_CORE_TRACE(LogModule::Scene, "Creating Scene ...");
    
    // Set the Scene state and register their corresponding Functions
    if (state_ == State::Edit)
      EditScene();
    else if (state_ == State::Play)
      PlayScene();
    else
      IK_ASSERT(false, "Invalid State");
  }

  void Scene::PlayScene() {
    IK_CORE_TRACE(LogModule::Scene, "Scene is Set to Play");
    state_ = State::Play;
  }
  
  void Scene::EditScene() {
    IK_CORE_TRACE(LogModule::Scene, "Scene is Set to Edit");
    state_ = State::Edit;
  }
  
} // namespace ikan
