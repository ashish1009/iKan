//
//  scene.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include <entt.hpp>
#include <box2d/b2_world.h>

#include "physics/contact_listener.hpp"
#include "physics/ray_cast.hpp"
#include "physics/debug_draw.hpp"
#include "core/utils/time_step.h"
#include "core/events/event.h"
#include "camera/editor_camera.hpp"
#include "camera/scene_camera.hpp"

namespace ikan {
  
  class Entity;
  struct TransformComponent;
  struct Box2DColliderComponent;
  struct CircleColliiderComponent;
  struct RigidBodyComponent;
  struct PillBoxColliderComponent;

  struct SceneCameraData {
    SceneCamera* scene_camera = nullptr;
    glm::vec3 position;
    glm::mat4 transform_matrix;
    
    // TODO: For debugging only
    TransformComponent* transform_comp;
    
    SceneCameraData() = default;
    DELETE_COPY_MOVE_CONSTRUCTORS(SceneCameraData);
  };

  class Scene {
  public:
    enum State : uint8_t {
      Play, Edit
    };
    enum Type : uint8_t {
      _2D = 0, _3D = 1
    };

    struct Setting {
      bool debug_draw = true;
      bool use_editor_camera = true;
      bool editor_camera = true;
    };

    /// This Constructor creates the instance of Scene.
    /// - Parameters:
    ///   - file_path: optional file path if we want to create a pre saved scene
    ///   - max_entity_capacity: Max entites memory to reserve in scene registry
    Scene(const std::string& file_path = "Unsaved_Scene", uint32_t max_entity_capacity = 200000);
    ~Scene();
    
    /// This function create and Entity and store in scene registry
    /// - Parameters:
    ///   - name: name of entity
    ///   - uuid: Unique ID of entity
    [[nodiscard]] Entity& CreateEntity(const std::string& name = "Unknown Entity", UUID uuid = UUID());
    /// This function destory the entity from scene registry
    /// - Parameter entity: entity to be destroyed
    void DestroyEntity(Entity entity);
    /// This function duplicate the entity and copy all the components
    /// - Parameter entity: entity to be destroyed
    [[nodiscard]] Entity& DuplicateEntity(Entity entity);

    /// This function update the scene
    /// - Parameter ts: time step
    void Update(Timestep ts);
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    void RenderGui();
    /// This dunction handles the events of the scene
    /// - Parameter e: event triggered
    void EventHandler(Event& e);
    
    /// This function update the viewport of the scene
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewport(uint32_t width, uint32_t height);

    /// This function sets the Scene as play mode
    /// - Parameter reset_physics: flag to reset Physics
    void PlayScene(bool reset_physics);
    /// This function sets the Scene as edit mode
    void EditScene();

    /// This function update the scene path
    /// - Parameter file_path: file path
    void SetFilePath(const std::string& file_path);
    /// This function change the renderer type of scene
    /// - Parameter type: type of scene
    void SetType(Type type);

    /// This function adds the Rigid body to the physics worlds
    /// - Parameters:
    ///   - entity: Entity
    ///   - rb2d: Rigid Body reference
    void AddBodyToPhysicsWorld(Entity entity, RigidBodyComponent& rb2d);
    /// This function Update the physics each frame
    /// - Parameter ts: time step
    void UpdatePhysics(Timestep ts);

    /// This function returns the entity Ref from its id
    /// - Parameter id: entity ID
    Entity* GetEnitityFromId(int32_t id);
    /// This function returns the state of scene
    State GetState() const { return state_; }
    /// This finction return is scene is in edit state
    bool IsEditing() const { return state_ == Edit; }
    /// This finction return Name of scene
    const std::string& GetName() const { return name_; }
    /// This finction return File path of scene
    const std::string& GetFilePath() const { return file_path_; }
    /// This function returns the type of scene
    Type GetType() const { return type_; }
    /// This function returns the editor camera of scene
    const EditorCamera& GetEditorCamera() const { return editor_camera_; };
    /// This function returns the setting reference to change the setting
    Setting& GetSetting() { return setting_; };
    /// This function returns the primary camera data
    const SceneCameraData& GetPrimaryCameraData() const { return primary_camera_data_; }
    /// This function returns the number of Entities stored in Scene
    uint32_t GetNumEntities() const { return num_entities_; }
    /// This function returns the Max Entity ID given to scene
    uint32_t GetMaxEntityId() const { return max_entity_id_; }
    /// This function returns the flag of use editor camera
    bool IsEditorCameraEnabled() const { return setting_.use_editor_camera; }
    /// This function returns the Pgysics 2D Gravity
    b2Vec2 Get2DWorldGravity() const { return physics_2d_world_->GetGravity(); }

    /// This function check is entity present in the scene as we delete the entity from scene in other frame
    /// - Parameter entity: eneity
    bool IsEntityPresentInMap(entt::entity entity) const;

    /// This function checks the position is on ground or not
    /// - Parameters:
    ///   - entity: entity pointer
    ///   - width: width of entity
    ///   - height: height of entity
    bool CheckOnGround(Entity* entity, float width, float height);
    /// This function callback the world raycast
    /// - Parameters:
    ///   - requesting_obj: requesting object type
    ///   - hit_point: hit point
    ///   - normal: normal of ray
    std::shared_ptr<RayCast2DInfo> RayCast2D(Entity* requesting_obj, const glm::vec2& hit_point, const glm::vec2& normal);
    
    /// This function create new scene copy the scene data from argument
    /// - Parameter other: copy scene
    static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);

    /// This function reset the ficture in body
    /// - Parameter body:
    static void ResetFixture(b2Body* body);
    /// This function reset the Pill Box collider fixture
    /// - Parameters:
    ///   - tc: transform oif body
    ///   - rb: rigid body
    ///   - pbc: pill box
    static void ResetPillBoxColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const PillBoxColliderComponent& pbc);
    /// This function reset the  Box collider fixture
    /// - Parameters:
    ///   - tc: transform oif body
    ///   - rb: rigid body
    ///   - pbc: box box
    static void ResetBoxColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const Box2DColliderComponent &pbc);
    /// This function reset the Circle collider fixture
    /// - Parameters:
    ///   - tc: transform oif body
    ///   - rb: rigid body
    ///   - pbc: circle box
    static void ResetCircleColliderFixture(const TransformComponent &tc, RigidBodyComponent* rb, const CircleColliiderComponent &pbc);
    
    /// This function add the box collider data to worlds body
    /// - Parameters:
    ///   - tc: transform of entity
    ///   - bc2d: box data
    ///   - body: body
    ///   - is_pill: Is Pill Box
    static void AddBoxColliderData(const TransformComponent& tc, const Box2DColliderComponent& bc2d, const RigidBodyComponent& rb2d,
                                   bool is_pill = false);
    /// This function add the circle collider data to worlds body
    /// - Parameters:
    ///   - tc: transform of entity
    ///   - cc2d: circlw data
    ///   - body: body
    ///   - is_pill: is Pill Box
    static void AddCircleColliderData(const TransformComponent& tc, const CircleColliiderComponent& cc2d, const RigidBodyComponent& rb2d,
                                      bool is_pill = false);
    
    /// This function add the circle collider data to worlds body
    /// - Parameters:
    ///   - tc: transform of entity
    ///   - cc2d: circlw data
    ///   - body: body
    ///   - is_pill: is Pill Box
    static void AddPillColliderData(const TransformComponent& tc, const PillBoxColliderComponent& pbc, const RigidBodyComponent& rb2d);
    
    /// This function returns the fxture size of body
    /// - Parameter body: physics body
    static int32_t FixtureListSize(b2Body* body);
    
    template<typename... Components>
    /// This function returns the entities with the components
    auto GetEntitesWith() {
      return registry_.view<Components...>();
    }

  private:
    // Member Functions
    /// This function creates an unique entity with UUID
    /// - Parameter uuid: Unique ID of entity
    Entity CreateUniqueEntity(UUID uuid);

    /// This function updates the scene in edit mode
    /// - Parameter ts time step
    void UpdateEditor(Timestep ts);
    /// This function updates the scene in play
    /// - Parameter ts time step
    void UpdateRuntime(Timestep ts);
    
    /// This function handles the event of the scene in edit mode
    /// - Parameter event event triggered
    void EventHandlerEditor(Event& event);
    /// This function handles the event of the scene in play mode
    /// - Parameter event event triggered
    void EventHandlerRuntime(Event& event);
    
    /// This function renderes the imgui of the scene in edit mode
    void RenderGuiEditor();
    /// This function renderes the imgui of the scene in play mode
    void RenderGuiRuntime();

    /// This function renders the 2D Entities
    /// - Parameter came_view_proj_mat: camera view projection matrix
    void Render2DEntities(const glm::mat4& came_view_proj_mat);
    /// This function renders the Texts
    /// - Parameter came_view_proj_mat: camera view projection matrix
    void RenderTexts(const glm::mat4& came_view_proj_mat);
    /// This function updates the primary camera data
    void UpdatePrimaryCameraData();
    /// This functuion Insantiates all the native script
    /// - Parameter ts: time step of each frame
    void InstantiateScript(Timestep ts);
    /// This function starts the runtime physics
    void RuntimeStart();

    /// This function removes the runtime fixture from colliders
    void RemoveRuntimeFixtureToColliders();
    
    // Member Variables
    // Utils Data
    std::string file_path_ = "Unsaved_Scene", name_ = "Unsaved_Scene";
    
    // EnTT data
    entt::registry registry_;
    std::unordered_map<entt::entity, Entity> entity_id_map_;
    uint32_t num_entities_ = 0;
    int32_t max_entity_id_ = -1;
    uint32_t curr_registry_capacity = 0;

    // Scene Data
    State state_ = State::Edit;
    Type type_ = Type::_2D;
    Setting setting_;
    
    // Camera Data
    SceneCameraData primary_camera_data_;
    EditorCamera editor_camera_;
    
    // Physics
    std::shared_ptr<b2World> physics_2d_world_;
    std::shared_ptr<b2ContactListener> contact_listner_2d_;
    std::shared_ptr<b2Draw> debug_draw_;

    friend class SceneSerializer;
    friend class ScenePanelManager;
    friend class Entity;
  };
  
} // namespace ikan
