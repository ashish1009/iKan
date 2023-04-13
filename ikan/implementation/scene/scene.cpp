//
//  scene.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "scene.hpp"
#include "components.hpp"
#include "core_entity.hpp"
#include "renderer/utils/batch_2d_renderer.hpp"

namespace ikan {
  
  template<typename... Component>
  static void ReserveRegistry(ComponentGroup<Component...>, entt::registry& registry, int32_t capacity) {
    registry.reserve<Component...>(capacity);
  }

  std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other) {
    std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();
    return new_scene;
  }

  Scene::Scene(const std::string& file_path, uint32_t max_entity_capacity)
  : file_path_(file_path), name_(StringUtils::GetNameFromFilePath(file_path)), curr_registry_capacity(max_entity_capacity) {
    IK_CORE_TRACE(LogModule::Scene, "Creating Scene ...");
    IK_CORE_TRACE(LogModule::Scene, "  Path               {0}", file_path_);
    IK_CORE_TRACE(LogModule::Scene, "  Name               {0}", name_);
    IK_CORE_TRACE(LogModule::Scene, "  Registry Capacity  {0}", curr_registry_capacity);

    ReserveRegistry(AllComponents{}, registry_, curr_registry_capacity);

    // Set the Scene state and register their corresponding Functions
    if (state_ == State::Edit)
      EditScene();
    else if (state_ == State::Play)
      PlayScene();
    else
      IK_ASSERT(false, "Invalid State");
  }
  
  Scene::~Scene() {
    IK_CORE_TRACE(LogModule::Scene, "Destroying Scene!!!");
    IK_CORE_TRACE(LogModule::Scene, "  Path               {0}", file_path_);
    IK_CORE_TRACE(LogModule::Scene, "  Name               {0}", name_);
    IK_CORE_TRACE(LogModule::Scene, "  Registry Capacity  {0}", curr_registry_capacity);
  }
  
  Entity Scene::CreateEntity(const std::string& name, UUID uuid) {
    Entity entity = CreateUniqueEntity(uuid);
    return entity;
  }

  Entity Scene::CreateUniqueEntity(UUID uuid) {
    if (registry_.size() >= curr_registry_capacity) {
      IK_ASSERT(false, "Temp Assert to check performance")
      curr_registry_capacity *= 2;
      ReserveRegistry(AllComponents{}, registry_, curr_registry_capacity);
    }
    
    Entity entity {registry_.create(), this};

    // Assert if this entity id is already present in scene entity map
    IK_ASSERT((entity_id_map_.find(entity) == entity_id_map_.end()), "Entity Already Added");

    // Add the Mendatory components
    entity.AddComponent<IDComponent>(uuid);

    // Store the entity in the entity uuid map. We Copy the Entity
    entity_id_map_[entity] = entity;

    // Updating the Max entity ID
    max_entity_id_ = (int32_t)((uint32_t)entity);

    ++num_entities_;

    return entity;
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
    if (setting_.use_editor_camera) {
      editor_camera_.Update(ts);
      
      if (type_ == _2D) {
        Render2DEntities(editor_camera_.GetViewProjection());
      }
      else if (type_ == _3D) {
      }
    }
  }
  
  void Scene::UpdateRuntime(Timestep ts) {
  }
  
  void Scene::Render2DEntities(const glm::mat4& cam_view_proj_mat) {
    Batch2DRenderer::BeginBatch(editor_camera_.GetViewProjection());
    Batch2DRenderer::DrawQuad(glm::mat4(1.0f), {1, 1, 1, 1});
    Batch2DRenderer::EndBatch();    
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
    editor_camera_.EventHandler(event);
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
    if (setting_.use_editor_camera) {
      editor_camera_.RendererGui(&setting_.editor_camera);
    }
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

  void Scene::SetType(Type type) {
    type_ = type;
    IK_CORE_TRACE(LogModule::Scene, "Scene Renderer Type is set to {0}", type_ == _2D ? "2D" : "3D");
    
    if (type_ == _2D) {
      setting_.use_editor_camera = false;
    }
  }

} // namespace ikan
