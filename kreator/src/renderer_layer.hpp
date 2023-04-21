//
//  renderer_layer.hpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "game_data.hpp"

namespace kreator {
  
  using namespace ikan;
  
  class RendererLayer : public Layer {
  public:
    struct SettingWrapper {
      std::string name;
      bool flag = true;
      SettingWrapper(const std::string& name, bool flag) : name(name), flag(flag) {}
      
      void ShowInMenu() {
        if (ImGui::MenuItem(name.c_str(), nullptr, flag, true)) {
          flag = (flag) ? false : true;
        }
      }
      void CheckBox() {
        PropertyGrid::CheckBox(name.c_str(), flag, 3 * ImGui::GetWindowContentRegionMax().x / 4);
      }
    };

    struct Setting {
      SettingWrapper show_collider{"Show Colliders", true};
      SettingWrapper content_browser_panel{"Content Browser Panel", true};
      SettingWrapper viewport_data{"Viewport Data", true};
      SettingWrapper frame_rate{"Frame Rate", false};
      SettingWrapper common_renderer_stats{"Common Renderer Stats", true};
      SettingWrapper renderer_stats_2d{"2D Renderer Stats", true};
      SettingWrapper save_scene{"Save Scene Panel", true};
      SettingWrapper prefab_loader{"Prefab Loader Panel", true};
    };
    
    /// Layer Default Constructor to store the name of layer
    /// - Parameter game_type: Type of game to render using this layer
    RendererLayer(GameType game_type);
    /// Default Destructor
    ~RendererLayer();
    
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    void Attach() override;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    void Detach() override;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Note: Core Application is taking care to call the RenderGui API for all Layers
    void RenderGui() override;
    /// This function updates the layer data. To be called each frame from application.
    /// - Note Core Application is taking care to call the Update(ts) API for all Layers
    /// - Parameter ts: Time step between 2 Frames
    void Update(Timestep ts) override;
    /// This function handles all the events triggered for the layer. To be whenever an event is triggered in the window.
    /// - Note Core Application is taking care to call the EventHandler(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void HandleEvents(Event& event) override;
    
    /// This function returns the playing state of game
    bool IsPlaying() const { return is_playing_; }
    
  private:
    enum Direction {Left, Right, Up, Down};

    /// This function renders the scene data
    void RenderScene(Timestep ts);
    /// This function render the viewport captured in framebuffer
    void RenderViewport();
    /// This function renders the debug overlay
    void OverlayRender();
    /// This function renders the Colliders
    void RenderColliders();
    
    /// This function handles the key button press event
    /// - Parameter e key button pressed event
    bool KeyPressed(KeyPressedEvent& event);
    /// This function handles the Window resize event
    /// - Parameter e window resize event
    bool WindowResized(WindowResizeEvent& event);
    /// This function handles the mouse button event
    /// - Parameter e: mouse button pressed event
    bool MouseButtonPressed(MouseButtonPressedEvent& event);

    /// This function update the size of all member present in application
    /// - Parameters:
    ///   - width: new width of viewport
    ///   - height: new height of viewport
    void ResizeData(uint32_t width, uint32_t height);
    
    /// This function render the menue bar
    void ShowMenu();
    /// This function renders the setting Widget
    void ShowSettings();
    /// This function renders the scene Play/Pause/Stop buttons
    void SceneStateButton();
    /// This function renders the Drop menu for scene type
    void SceneRendererType();

    /// This function is the wrapper to creates a new scene instance
    /// - Parameter scene_path: path of new scene
    void NewScene(const std::string& scene_path = "Unsaved Scene");
    /// This function creates a new scene instance
    /// - Parameter scene_path: path of new scene
    void NewSceneImpl(const std::string& scene_path = "Unsaved Scene");
    /// This function Renders an Imgui widget that helps in saving scene
    void SaveScene();
    /// This function closes the current scene
    void CloseScene();
    /// This function opens already saved scene from path
    /// - Parameter scene_file_path: scene file path
    const bool OpenScene(const std::string& scene_file_path);

    /// This function set the game state
    /// - Parameter is_play: game state
    void SetPlay(bool is_play);
    /// This function wraps the scene play
    void PlayScene();
    /// This function wraps the scene edit in same state of playing
    void EditScene();
    /// This function wraps the scene Stop. Edit from begining
    void StopScene();

    /// This function Renders the grid for camera
    void RenderGrid();
    /// This function render the rectangle when click drag mouse and select the entities
    void SelectEntities();
    /// This function clear the selected entities
    void ClearSelectedEntities();
    /// This function highlight the selected entiies
    /// - Parameter enable: flag to set or unset
    void HighlightSelectedEntities(bool enable);
    /// This function Delete the selected entities
    void DeleteSelectedEntities();
    /// This function Duplicate the selected entities
    void DuplicateSelectedEntities();
    /// This function moves the selected entities
    /// - Parameter direction: Direction of movement
    void MoveEntities(Direction direction);

    // Member variables
    uint32_t viewport_width_ = Application::Get().GetWindow().GetWidth();
    uint32_t viewport_height_ = Application::Get().GetWindow().GetWidth();

    bool is_playing_ = false;
    bool show_setting_ = true;
    std::unique_ptr<GameData> game_data_;
    Setting setting_;
    Viewport viewport_;
    
    std::shared_ptr<Scene> active_scene_, editor_scene_;
    ContentBrowserPanel cbp_;
    ScenePanelManager spm_;

    // Debug Data
    std::unordered_map<entt::entity, Entity*> selected_entities_;
  };
  
} // namespace kreator
