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
  
  Entity& Scene::CreateEntity(const std::string& name, UUID uuid) {
    Entity entity = CreateUniqueEntity(uuid);
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>();

    // Debug Logs
    IK_CORE_TRACE(LogModule::Scene, "Stored Entity in Scene");
    IK_CORE_TRACE(LogModule::Scene, "  Name    {0}", entity.GetComponent<TagComponent>().tag.c_str());
    IK_CORE_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_CORE_TRACE(LogModule::Scene, "  ID      {0}", entity.GetComponent<IDComponent>().id);
    IK_CORE_TRACE(LogModule::Scene, "  Number of entities Added in Scene  {0}", num_entities_);
    IK_CORE_TRACE(LogModule::Scene, "  Max ID given to entity             {0}", max_entity_id_);

    return entity_id_map_.at((entt::entity)entity);
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
  
  void Scene::DestroyEntity(Entity entity) {
    IK_CORE_WARN(LogModule::Scene, "Removed Entity from Scene");
    IK_CORE_WARN(LogModule::Scene, "  Name    {0}", entity.GetComponent<TagComponent>().tag.c_str());
    IK_CORE_WARN(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_CORE_WARN(LogModule::Scene, "  ID      {0}", entity.GetComponent<IDComponent>().id);
    IK_CORE_WARN(LogModule::Scene, "  Number of entities left in the Scene  {0}", --num_entities_);
   
    // Delete the eneity from the map
    entity_id_map_.erase(entity);
    registry_.destroy(entity);
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
    Batch2DRenderer::BeginBatch(cam_view_proj_mat);
    
    // Render All Quad Entities
    auto quad_view = registry_.view<TransformComponent, QuadComponent>();
    for (const auto& quad_entity : quad_view) {
      const auto& [transform_component, quad_component] = quad_view.get<TransformComponent, QuadComponent>(quad_entity);
      Batch2DRenderer::DrawQuad(transform_component.Transform(), {1, 1, 1, 1});
    } // For each Quad Entity
    
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

  void Scene::SetViewport(uint32_t width, uint32_t height) {
    editor_camera_.SetViewportSize(width, height);
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
