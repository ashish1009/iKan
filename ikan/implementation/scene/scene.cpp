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

  Scene::Scene(const std::string& file_path)
  : file_path_(file_path), name_(StringUtils::GetNameFromFilePath(file_path)) {
    IK_CORE_TRACE(LogModule::Scene, "Creating Scene ...");
    IK_CORE_TRACE(LogModule::Scene, "  Path | {0}", file_path_);
    IK_CORE_TRACE(LogModule::Scene, "  Name | {0}", name_);

    // Set the Scene state and register their corresponding Functions
    if (state_ == State::Edit)
      EditScene();
    else if (state_ == State::Play)
      PlayScene();
    else
      IK_ASSERT(false, "Invalid State");
  }
  
  void Scene::Update(Timestep ts) {
    if (IsEditing()) {
      UpdateEditor(ts);
    }
    else {
      UpdateRuntime(ts);
    }
  }
  
  void Scene::UpdateEditor(Timestep ts) {
  }
  
  void Scene::UpdateRuntime(Timestep ts) {
  }
  
  void Scene::EventHandler(Event& event) {
    if (IsEditing()) {
      EventHandlerEditor(event);
    }
    else {
      EventHandlerRuntime(event);
    }
  }
  
  void Scene::EventHandlerEditor(Event& event) {
  }
  
  void Scene::EventHandlerRuntime(Event& event) {
  }
  
  void Scene::RenderGui() {
    if (IsEditing()) {
      RenderGuiEditor();
    }
    else {
      RenderGuiRuntime();
    }
  }
  
  void Scene::RenderGuiEditor() {
  }
  
  void Scene::RenderGuiRuntime() {
  }

  void Scene::PlayScene() {
    IK_CORE_TRACE(LogModule::Scene, "Scene is Set to Play");
    state_ = State::Play;
  }
  
  void Scene::EditScene() {
    IK_CORE_TRACE(LogModule::Scene, "Scene is Set to Edit");
    state_ = State::Edit;
  }

  void Scene::SetFilePath(const std::string& file_path) {
    file_path_ = file_path;
    name_ = StringUtils::GetNameFromFilePath(file_path_);
  }

} // namespace ikan
