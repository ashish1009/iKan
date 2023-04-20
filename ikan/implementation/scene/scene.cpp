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
  
  template<typename... Component>
  static void CopyComponent(entt::registry& dst,
                            entt::registry& src,
                            const std::unordered_map<UUID, entt::entity>& enttMap) {
    ([&]()
     {
      auto view = src.view<Component>();
      for (auto src_entity : view) {
        entt::entity dst_entity = enttMap.at(src.get<IDComponent>(src_entity).id);
        
        auto& srcComponent = src.get<Component>(src_entity);
        dst.emplace_or_replace<Component>(dst_entity, srcComponent);
      }
    }(), ...);
  }
  
  template<typename... Component>
  static void CopyComponent(ComponentGroup<Component...>,
                            entt::registry& dst,
                            entt::registry& src,
                            const std::unordered_map<UUID, entt::entity>& entt_map) {
    CopyComponent<Component...>(dst, src, entt_map);
  }
  
  template<typename... Component>
  static void CopyComponentIfExists(Entity dst, Entity src) {
    ([&]()
     {
      if (src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
    }(), ...);
  }
  
  template<typename... Component>
  static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
    CopyComponentIfExists<Component...>(dst, src);
  }
  
  template<typename Component>
  static void CopySingleComponentIfExists(Entity& dst, Entity& src) {
    if (src.HasComponent<Component>())
      dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
  }

  std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other) {
    std::shared_ptr<Scene> new_scene = std::make_shared<Scene>(other->GetFilePath());
    new_scene->setting_= other->setting_;
    new_scene->type_ = other->type_;

    auto& src_scene_registry = other->registry_;
    auto& dst_scene_registry = new_scene->registry_;
    std::unordered_map<UUID, entt::entity> entt_map;

    // Create entities in new scene
    auto id_view = src_scene_registry.view<IDComponent>();
    for (auto e : id_view) {
      UUID uuid = src_scene_registry.get<IDComponent>(e).id;
      const auto& name = src_scene_registry.get<TagComponent>(e).tag;
      Entity new_entity = new_scene->CreateEntity(name, uuid);
      entt_map[uuid] = (entt::entity)new_entity;
    }
    
    // Copy components (except IDComponent and TagComponent)
    CopyComponent(AllCopyComponents{}, dst_scene_registry, src_scene_registry, entt_map);

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
  
  Entity& Scene::DuplicateEntity(Entity entity) {
    Entity new_entity = CreateUniqueEntity(UUID());
    CopySingleComponentIfExists<TagComponent>(new_entity, entity);
    CopyComponentIfExists(AllCopyComponents{}, new_entity, entity);
    
    // Debug Logs
    IK_CORE_TRACE(LogModule::Scene, "Stored Entity in Scene");
    IK_CORE_TRACE(LogModule::Scene, "  Name    {0}", new_entity.GetComponent<TagComponent>().tag.c_str());
    IK_CORE_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)new_entity);
    IK_CORE_TRACE(LogModule::Scene, "  ID      {0}", new_entity.GetComponent<IDComponent>().id);
    IK_CORE_TRACE(LogModule::Scene, "  Number of entities Added in Scene {0}", num_entities_);
    IK_CORE_TRACE(LogModule::Scene, "  Max ID given to entity            {0}", max_entity_id_);
    
    return entity_id_map_.at((entt::entity)entity);
  }
  
  void Scene::Update(Timestep ts) {
    UpdatePrimaryCameraData();

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
    else {
      UpdateRuntime(ts);
    }
  }
  
  void Scene::UpdateRuntime(Timestep ts) {
    if (primary_camera_data_.scene_camera) {
      if (type_ == _2D) {
        Render2DEntities(primary_camera_data_.scene_camera->GetProjection() * glm::inverse(primary_camera_data_.transform_matrix));
      }
      else if (type_ == _3D) {
      }
    }
  }
  
  void Scene::Render2DEntities(const glm::mat4& cam_view_proj_mat) {
    Batch2DRenderer::BeginBatch(cam_view_proj_mat);
    
    auto circle_view = registry_.view<TransformComponent, CircleComponent>();
    // For all circle entity
    for (const auto& circle_entity : circle_view) {
      const auto& [transform_component, circle_component] = circle_view.get<TransformComponent, CircleComponent>(circle_entity);
      if (circle_component.texture_comp.use and circle_component.texture_comp.texture) {
        Batch2DRenderer::DrawCircle(transform_component.Transform(), circle_component.texture_comp.texture, circle_component.color,
                                    circle_component.texture_comp.tiling_factor, circle_component.thickness, circle_component.fade, (uint32_t)circle_entity);
        
      } else {
        Batch2DRenderer::DrawCircle(transform_component.Transform(), circle_component.color, circle_component.thickness, circle_component.fade,
                                    (uint32_t)circle_entity);
      }
    } // for (const auto& entity : circle_view)

    // Render All Quad Entities
    auto quad_view = registry_.view<TransformComponent, QuadComponent>();
    for (const auto& quad_entity : quad_view) {
      const auto& [transform_comp, quad_comp] = quad_view.get<TransformComponent, QuadComponent>(quad_entity);
      if (quad_comp.sprite.use and quad_comp.sprite.texture) {
        // Sprite Animation
        if (quad_comp.sprite.type == SpriteComponent::Type::Animation and quad_comp.sprite.sprites.size() > 0) {
          if (quad_comp.sprite.anim_idx >= quad_comp.sprite.speed * quad_comp.sprite.sprites.size() or quad_comp.sprite.anim_idx < 1)
            quad_comp.sprite.anim_idx = 0;
          Batch2DRenderer::DrawQuad(transform_comp.Transform(), quad_comp.sprite.sprites[quad_comp.sprite.anim_idx / quad_comp.sprite.speed],
                                    quad_comp.color, (uint32_t)quad_entity);
          quad_comp.sprite.anim_idx++;
        }
        // Sprite No Animation
        else if (quad_comp.sprite.type == SpriteComponent::Type::Sprite) {
          Batch2DRenderer::DrawQuad(transform_comp.Transform(), quad_comp.sprite.sub_texture, quad_comp.color, (uint32_t)quad_entity);
        }
        // Only Texture
        else {
          Batch2DRenderer::DrawQuad(transform_comp.Transform(), quad_comp.sprite.texture, quad_comp.color, quad_comp.sprite.tiling_factor,
                                    (uint32_t)quad_entity);
        }
      }
      else {
        Batch2DRenderer::DrawQuad(transform_comp.Transform(), quad_comp.color, (uint32_t)quad_entity);
      }
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
    
    // Update the viewport of scene cameras
    auto view = registry_.view<CameraComponent>();
    for (auto entity : view) {
      auto& cc = view.get<CameraComponent>(entity);
      if (!cc.is_fixed_aspect_ratio)
        cc.camera->SetViewportSize(width, height);
    }
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
  
  void Scene::UpdatePrimaryCameraData() {
    auto camera_view = registry_.view<TransformComponent, CameraComponent>();
    for (auto& camera_entity : camera_view) {
      const auto& [transform_component, camera_component] = camera_view.get<TransformComponent, CameraComponent>(camera_entity);
      if (camera_component.is_primary) {
        primary_camera_data_.scene_camera = camera_component.camera.get();
        primary_camera_data_.position = transform_component.Position();
        primary_camera_data_.transform_matrix = transform_component.Transform();
        
        primary_camera_data_.transform_comp = &transform_component;
        return;
      }
    }
    primary_camera_data_.scene_camera = nullptr;
  }

  Entity* Scene::GetEnitityFromId(int32_t id) {
    if (entity_id_map_.find((entt::entity)id) != entity_id_map_.end())
      return &entity_id_map_.at((entt::entity)id);
    return nullptr;
  }

} // namespace ikan
