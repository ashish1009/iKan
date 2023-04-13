//
//  scene.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include <entt.hpp>
#include "core/utils/time_step.h"
#include "core/events/event.h"
#include "camera/editor_camera.hpp"

namespace ikan {
  
  class Entity;
  
  class Scene {
  public:
    enum State : uint8_t {
      Play, Edit
    };
    enum Type : uint8_t {
      _2D = 0, _3D = 1
    };

    struct Setting {
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
    [[nodiscard]] Entity CreateEntity(const std::string& name = "Unknown Entity", UUID uuid = UUID());
    /// This function destory the entity from scene registry
    /// - Parameter entity: entity to be destroyed
    void DestroyEntity(Entity entity);

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
    void PlayScene();
    /// This function sets the Scene as edit mode
    void EditScene();

    /// This function update the scene path
    /// - Parameter file_path: file path
    void SetFilePath(const std::string& file_path);
    /// This function change the renderer type of scene
    /// - Parameter type: type of scene
    void SetType(Type type);

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

    /// This function create new scene copy the scene data from argument
    /// - Parameter other: copy scene
    static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);

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

    // Member Variables
    std::string file_path_ = "Unsaved_Scene", name_ = "Unsaved_Scene";
    
    entt::registry registry_;
    std::unordered_map<entt::entity, Entity> entity_id_map_;

    uint32_t num_entities_ = 0;
    int32_t max_entity_id_ = -1;
    uint32_t curr_registry_capacity = 0;

    State state_ = State::Edit;
    Type type_ = Type::_2D;
    
    EditorCamera editor_camera_;

    Setting setting_;

    friend class SceneSerializer;
    friend class ScenePanelManager;
    friend class Entity;
  };
  
} // namespace ikan
