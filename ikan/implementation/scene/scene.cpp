//
//  scene.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "scene.hpp"
#include "scene/components.hpp"
#include "scene/core_entity.hpp"
#include "renderer/utils/batch_2d_renderer.hpp"
#include "renderer/utils/text_renderer.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

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
      PlayScene(false);
    else
      IK_ASSERT(false, "Invalid State");
  }
  
  Scene::~Scene() {
    IK_CORE_TRACE(LogModule::Scene, "Destroying Scene!!!");
    IK_CORE_TRACE(LogModule::Scene, "  Path               {0}", file_path_);
    IK_CORE_TRACE(LogModule::Scene, "  Name               {0}", name_);
    IK_CORE_TRACE(LogModule::Scene, "  Registry Capacity  {0}", curr_registry_capacity);
    
    RemoveRuntimeFixtureToColliders();
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
   
    // Delete physics data
    if (physics_2d_world_ and entity.HasComponent<RigidBodyComponent>()) {
      auto& rb = entity.GetComponent<RigidBodyComponent>();
      ResetFixture((b2Body*)rb.runtime_body);
      
      physics_2d_world_->DestroyBody((b2Body*)rb.runtime_body);
      rb.runtime_body = nullptr;
    }

    // Destory the Scripts data
    if (entity.HasComponent<NativeScriptComponent>()) {
      auto& nsc = entity.GetComponent<NativeScriptComponent>();
      nsc.script->Destroy();
    }
    
    if (type_ == _2D) {
      if (entity.HasComponent<CircleColliiderComponent>()) {
        auto& cc = entity.GetComponent<CircleColliiderComponent>();
        delete cc.runtime_fixture;
        cc.runtime_fixture = nullptr;
      }
      
      if (entity.HasComponent<Box2DColliderComponent>()) {
        auto& bc = entity.GetComponent<Box2DColliderComponent>();
        delete bc.runtime_fixture;
        bc.runtime_fixture = nullptr;
      }
      
      if (entity.HasComponent<PillBoxColliderComponent>()) {
        auto& pbc = entity.GetComponent<PillBoxColliderComponent>();
        delete pbc.bcc.runtime_fixture;
        pbc.bcc.runtime_fixture = nullptr;
        delete pbc.top_ccc.runtime_fixture;
        pbc.top_ccc.runtime_fixture = nullptr;
        delete pbc.bottom_ccc.runtime_fixture;
        pbc.bottom_ccc.runtime_fixture = nullptr;
      }
    }
    
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
      InstantiateScript(ts);
      UpdatePhysics(ts);
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
      RenderTexts(editor_camera_.GetViewProjection());
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
      RenderTexts(primary_camera_data_.scene_camera->GetProjection() * glm::inverse(primary_camera_data_.transform_matrix));
    }
  }
  
  void Scene::UpdatePhysics(Timestep ts) {
    if (type_ == _2D) {
      // Physics
      const int32_t velocity_iteration = 6;
      const int32_t position_iteration = 2;
      
      physics_2d_world_->Step(ts, velocity_iteration, position_iteration);
      
      // Get Transform
      auto view = registry_.view<RigidBodyComponent>();
      for (auto e : view) {
        Entity& entity = entity_id_map_.at(e);
        
        auto& rb2d = entity.GetComponent<RigidBodyComponent>();
        if (rb2d.type == RigidBodyComponent::RbBodyType::Dynamic or rb2d.type == RigidBodyComponent::RbBodyType::Kinametic) {
          auto& transform = entity.GetComponent<TransformComponent>();
          b2Body* body = (b2Body*)rb2d.runtime_body;
          if (body != nullptr) {
            const auto& position = body->GetPosition();
            
            transform.UpdateRotation(Z, body->GetAngle());
            transform.UpdatePosition({position.x, position.y, transform.Position().z});
          }
        } // if (rb2d.type == b2_dynamicBody or rb2d.type == b2_kinematicBody)
      } // for (auto e : view)
    } // if (type_ == _2D)
  }
  
  void Scene::Render2DEntities(const glm::mat4& cam_view_proj_mat) {
    Batch2DRenderer::BeginBatch(cam_view_proj_mat);
    
    auto circle_view = registry_.view<TransformComponent, CircleComponent>();
    // For all circle entity
    for (const auto& circle_entity : circle_view) {
      const auto& [transform_component, circle_component] = circle_view.get<TransformComponent, CircleComponent>(circle_entity);
      if (circle_component.texture_comp.use and circle_component.texture_comp.texture.size() > 0) {
        Batch2DRenderer::DrawCircle(transform_component.Transform(), circle_component.texture_comp.texture.at(0), circle_component.color,
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
      auto& sprite = quad_comp.sprite;
      
      if (sprite.use and sprite.texture.size() > 0) {
        // Sprite
        if (sprite.use_sub_texture) {
          // Sprite Animation
          if (sprite.sprite_images.size() > 1) {
            sprite.ResetAnimIndx();
            Batch2DRenderer::DrawQuad(transform_comp.Transform(), sprite.sprite_images[sprite.anim_idx / sprite.speed], quad_comp.color,
                                      (uint32_t)quad_entity);
            sprite.anim_idx++;
          }
          // Sprite No Animation
          else if (sprite.sprite_images.size() == 1) {
            Batch2DRenderer::DrawQuad(transform_comp.Transform(), sprite.sprite_images[0], quad_comp.color, (uint32_t)quad_entity);
          }
        }
        // Only Texture
        else {
          // Sprite Animation
          if (sprite.texture.size() > 1) {
            sprite.ResetAnimIndx();
            Batch2DRenderer::DrawQuad(transform_comp.Transform(), sprite.texture[sprite.anim_idx / sprite.speed], quad_comp.color, sprite.tiling_factor,
                                      (uint32_t)quad_entity);
            sprite.anim_idx++;
          }
          // Sprite No Animation
          else if (sprite.texture.size() == 1) {
            Batch2DRenderer::DrawQuad(transform_comp.Transform(), sprite.texture.at(0), quad_comp.color, sprite.tiling_factor, (uint32_t)quad_entity);
          }
        }
      }
      else {
        Batch2DRenderer::DrawQuad(transform_comp.Transform(), quad_comp.color, (uint32_t)quad_entity);
      }
    } // For each Quad Entity
    
    if (setting_.debug_draw) {
      if (physics_2d_world_) {
        physics_2d_world_->DebugDraw();
      }
    }
    
    Batch2DRenderer::EndBatch();
  }
  
  void Scene::RenderTexts(const glm::mat4& cam_view_proj_mat) {
    TextRenderer::BeginBatch(cam_view_proj_mat);
    
    auto text_view = registry_.view<TransformComponent, TextComponent>();
    // For all text entity
    for (const auto& text_entity : text_view) {
      const auto& [transform_component, text_component] = text_view.get<TransformComponent, TextComponent>(text_entity);
      TextRenderer::RenderText(text_component.text, transform_component.Position(), transform_component.Scale(), text_component.color);
    } // for (const auto& entity : sprite_view)
    
    TextRenderer::EndBatch();
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
    registry_.view<NativeScriptComponent>().each([&](auto entity, auto& nsc) {
      if (nsc.script)
        nsc.script->EventHandler(event);
    });
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

  void Scene::PlayScene(bool reset_physics) {
    IK_CORE_TRACE(LogModule::Scene, "Scene is Set to Play");
    state_ = State::Play;
    
    if (reset_physics)
      RuntimeStart();
  }
  
  void Scene::RuntimeStart() {
    if (type_ == _2D) {
      physics_2d_world_ = std::make_shared<b2World>(b2Vec2(0.0f, -9.8f));
      
      contact_listner_2d_ = std::make_shared<ContactListner2D>();
      physics_2d_world_->SetContactListener(contact_listner_2d_.get());
      
      debug_draw_ = std::make_shared<DebugDraw>();
      debug_draw_->AppendFlags(DebugDraw::e_shapeBit);
      physics_2d_world_->SetDebugDraw(debug_draw_.get());
      
      auto view = registry_.view<RigidBodyComponent>();
      for (auto e : view) {
        Entity entity = { e, this };
        auto& rb2d = entity.GetComponent<RigidBodyComponent>();
        AddBodyToPhysicsWorld(entity,rb2d);
      }
    }
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

  void Scene::InstantiateScript(Timestep ts) {
    registry_.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                                                 {
      if (!nsc.script) {
        ScriptManager::UpdateScript(&nsc, nsc.script_name, nsc.loader_function);
      }
      else {
        if (!nsc.script->scene_) {
          nsc.script->scene_ = this;
          nsc.script->Create(Entity{ entity, this });
        }
        nsc.script->Update(ts);
      }
    });
  }
  
  int32_t Scene::FixtureListSize(b2Body* body) {
    int32_t size = 0;
    b2Fixture* fixture = body->GetFixtureList();
    while (fixture) {
      size++;
      fixture = fixture->GetNext();
    }
    return size;
  }

  void Scene::ResetFixture(b2Body* body) {
    int32_t size = FixtureListSize(body);
    for (int32_t i = 0; i < size; i++) {
      body->DestroyFixture(body->GetFixtureList());
    }
  }
  
  void Scene::ResetBoxColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const Box2DColliderComponent &pbc) {
    b2Body* body = (b2Body*)rb->runtime_body;
    ResetFixture(body);
    AddBoxColliderData(tc, pbc, *rb);
    body->ResetMassData();
  }
  
  void Scene::ResetCircleColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const CircleColliiderComponent &pbc) {
    b2Body* body = (b2Body*)rb->runtime_body;
    ResetFixture(body);
    AddCircleColliderData(tc, pbc, *rb);
    body->ResetMassData();
  }
  
  void Scene::ResetPillBoxColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const PillBoxColliderComponent &pbc) {
    b2Body* body = (b2Body*)rb->runtime_body;
    ResetFixture(body);
    AddPillColliderData(tc, pbc, *rb);
    body->ResetMassData();
  }
  
  void Scene::RemoveRuntimeFixtureToColliders() {
    // Store the Entity in each box collider
    auto box_view = registry_.view<Box2DColliderComponent>();
    for (auto e : box_view) {
      auto &c = box_view.get<Box2DColliderComponent>(e);
      delete c.runtime_fixture;
      c.runtime_fixture = nullptr;
    }
    
    // Store the Entity in each circle collider
    auto circle_view = registry_.view<CircleColliiderComponent>();
    for (auto e : circle_view) {
      auto &c = circle_view.get<CircleColliiderComponent>(e);
      delete c.runtime_fixture;
      c.runtime_fixture = nullptr;
    }
    
    auto pill_view = registry_.view<PillBoxColliderComponent>();
    for (auto e : pill_view) {
      auto &c = pill_view.get<PillBoxColliderComponent>(e);
      
      delete c.bcc.runtime_fixture;
      c.bcc.runtime_fixture = nullptr;
      delete c.top_ccc.runtime_fixture;
      c.top_ccc.runtime_fixture = nullptr;
      delete c.bottom_ccc.runtime_fixture;
      c.bottom_ccc.runtime_fixture = nullptr;
    }
  }
  
  void Scene::AddBoxColliderData(const TransformComponent& tc, const Box2DColliderComponent& bc2d, const RigidBodyComponent& rb2d, bool is_pill) {
    b2Body* body = (b2Body*)rb2d.runtime_body;
    b2PolygonShape polygon_shape;
    if (is_pill)
      polygon_shape.SetAsBox(bc2d.size.x, bc2d.size.y, {bc2d.offset.x, bc2d.offset.y}, 0);
    else
      polygon_shape.SetAsBox(bc2d.size.x * tc.Scale().x, bc2d.size.y * tc.Scale().y, {bc2d.offset.x, bc2d.offset.y}, 0);
    
    b2FixtureDef fixture_def;
    fixture_def.shape = & polygon_shape;
    fixture_def.density = bc2d.physics_mat.density;
    fixture_def.friction = bc2d.physics_mat.friction;
    fixture_def.restitution = bc2d.physics_mat.restitution;
    fixture_def.restitutionThreshold = bc2d.physics_mat.restitution_threshold;
    fixture_def.isSensor = rb2d.is_sensor;
    fixture_def.userData.pointer = (uintptr_t)bc2d.runtime_fixture;
    
    body->CreateFixture(&fixture_def);
  }
  
  void Scene::AddCircleColliderData(const TransformComponent& tc, const CircleColliiderComponent& cc2d, const RigidBodyComponent& rb2d, bool is_pill) {
    b2Body* body = (b2Body*)rb2d.runtime_body;
    b2CircleShape circle_shape;
    circle_shape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
    if (is_pill)
      circle_shape.m_radius = cc2d.radius;
    else
      circle_shape.m_radius = std::abs(tc.Scale().x) * cc2d.radius;
    
    b2FixtureDef fixture_def;
    fixture_def.shape = & circle_shape;
    fixture_def.density = cc2d.physics_mat.density;
    fixture_def.friction = cc2d.physics_mat.friction;
    fixture_def.restitution = cc2d.physics_mat.restitution;
    fixture_def.restitutionThreshold = cc2d.physics_mat.restitution_threshold;
    fixture_def.isSensor = rb2d.is_sensor;
    fixture_def.userData.pointer = (uintptr_t)cc2d.runtime_fixture;
    
    body->CreateFixture(&fixture_def);
  }
  
  void Scene::AddPillColliderData(const TransformComponent &tc, const PillBoxColliderComponent &pbc, const RigidBodyComponent& rb2d) {
    AddBoxColliderData(tc, pbc.bcc, rb2d, true);
    AddCircleColliderData(tc, pbc.top_ccc, rb2d, true);
    AddCircleColliderData(tc, pbc.bottom_ccc, rb2d, true);
  }
  
  void Scene::AddBodyToPhysicsWorld(Entity entity, RigidBodyComponent& rb2d) {
    auto& transform = entity.GetComponent<TransformComponent>();

    b2BodyDef body_def;
    body_def.type = RigidBodyComponent::B2BodyType(rb2d.type);
    body_def.position.Set(transform.Position().x, transform.Position().y);
    body_def.angle = transform.Rotation().z;
    
    body_def.linearVelocity = {rb2d.velocity.x, rb2d.velocity.y};
    body_def.angularVelocity = rb2d.angular_velocity;
    body_def.linearDamping = rb2d.linear_damping;
    body_def.angularDamping = rb2d.angular_damping;
    body_def.gravityScale = rb2d.gravity_scale;

    b2Body* body = physics_2d_world_->CreateBody(&body_def);
    body->SetFixedRotation(rb2d.fixed_rotation);
    rb2d.runtime_body = body;

    if (entity.HasComponent<Box2DColliderComponent>()) {
      auto& bc2d = entity.GetComponent<Box2DColliderComponent>();
      bc2d.runtime_fixture = new Entity(entity);
      AddBoxColliderData(transform, bc2d, rb2d);
    }
    
    if (entity.HasComponent<CircleColliiderComponent>()) {
      auto& cc2d = entity.GetComponent<CircleColliiderComponent>();
      cc2d.runtime_fixture = new Entity(entity);
      AddCircleColliderData(transform, cc2d, rb2d);
    }
    
    if (entity.HasComponent<PillBoxColliderComponent>()) {
      auto& pbc = entity.GetComponent<PillBoxColliderComponent>();
      pbc.bcc.runtime_fixture = new Entity(entity);
      pbc.top_ccc.runtime_fixture = new Entity(entity);
      pbc.bottom_ccc.runtime_fixture = new Entity(entity);
      AddPillColliderData(transform, pbc, rb2d);
    }
  }
  
  bool Scene::CheckOnGround(Entity* entity, float width, float height) {
    const glm::vec2& position = glm::vec2(entity->GetComponent<TransformComponent>().Position());
    
    glm::vec2 ray_cast_1_begin = position - glm::vec2(width / 2.0f, 0.0f);
    glm::vec2 ray_cast_1_end = ray_cast_1_begin + glm::vec2(0.0f, height);
    std::shared_ptr<RayCast2DInfo> info_1 = RayCast2D(entity, {ray_cast_1_begin.x, ray_cast_1_begin.y}, {ray_cast_1_end.x, ray_cast_1_end.y});
    
    glm::vec2 ray_cast_2_begin = ray_cast_1_begin + glm::vec2(width, 0.0f);
    glm::vec2 ray_cast_2_end = ray_cast_1_end + glm::vec2(width, 0.0f);
    std::shared_ptr<RayCast2DInfo> info_2 = RayCast2D(entity, {ray_cast_2_begin.x, ray_cast_2_begin.y}, {ray_cast_2_end.x, ray_cast_2_end.y});
    
    bool on_ground = info_1->OnGround() or info_2->OnGround();
    
    if (setting_.debug_draw) {
      const auto& cd = primary_camera_data_;
      Batch2DRenderer::BeginBatch(cd.scene_camera->GetProjection() * glm::inverse(cd.transform_matrix));
      Batch2DRenderer::DrawLine(glm::vec3(ray_cast_1_begin, 0.0f), glm::vec3(ray_cast_1_end, 0.0f), {0, 1, 0, 1});
      Batch2DRenderer::DrawLine(glm::vec3(ray_cast_2_begin, 0.0f), glm::vec3(ray_cast_2_end, 0.0f), {0, 1, 0, 1});
      Batch2DRenderer::EndBatch();
    }
    
    return on_ground;
  }

  std::shared_ptr<RayCast2DInfo> Scene::RayCast2D(Entity* requesting_obj, const glm::vec2& hit_point, const glm::vec2& normal) {
    std::shared_ptr<RayCast2DInfo> callback = std::make_shared<RayCast2DInfo>(requesting_obj);
    physics_2d_world_->RayCast(callback.get(), { hit_point.x, hit_point.y }, { normal.x, normal.y });
    return callback;
  }

  bool Scene::IsEntityPresentInMap(entt::entity entity) const { return entity_id_map_.find(entity) != entity_id_map_.end(); }

} // namespace ikan
