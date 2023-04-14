//
//  scene_panel_manager.hpp
//  ikan
//
//  Created by Ashish . on 12/04/23.
//

#pragma once
#include "core_entity.hpp"

namespace ikan {
  
  /// This class is the wrapper to render scene pannel
  class ScenePanelManager {
  public:
    struct Setting {
      bool scene_panel = true;
      bool property_panel = true;
    };
    
    /// This Constructor creates Scene pannel manager instance
    ScenePanelManager(Scene* context = nullptr);
    /// This Destructor destroys Scene pannel manager instance
    ~ScenePanelManager();
    
    /// This function renders imgui window for scene pannels
    void RenderGui();
    /// This function update the scene contexrt
    /// - Parameter context: scene context
    void SetSceneContext(Scene* context) { scene_context_ = context; }
    /// This Function update the selected entity of scene
    /// - Parameter entity: new selected entity pointer
    void SetSelectedEntity(Entity* entity);
    /// This function renders the option menu on right click the empty space
    void RightClickOptions();
    /// This function adds the component in the selected entity
    void AddComponent();

    /// This function returns the context of panel
    Scene* GetContext() { return scene_context_; }
    /// This function returns the reference of setting
    Setting& GetSetting() { return setting_; }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ScenePanelManager)
    
  private:
    // Member functions
    /// This function render a pannel for scene information
    void ScenePannel();
    /// This function renders the property for selected entityu in scene
    void PropertyPannel();

    /// This function draw the entity tree node
    /// - Parameter entity_id: entity id from scene registry
    void DrawEntityTreeNode(entt::entity entity_id);

    template <typename T, typename... Args>
    /// This function create menu for add component
    /// - Parameters:
    ///   - menu_item: menu name
    ///   - exclusive_func: exclusing function
    void AddComponentMenu(const std::string& menu_item, bool exclusive = false, Args&&... args) {
      if (ImGui::MenuItem(menu_item.c_str(), nullptr /* Shortcut */, false /* Selected */, exclusive)) {
        selected_entity_->AddComponent<T>(std::forward<Args>(args)...);
        ImGui::CloseCurrentPopup();
      }
    }

    // Member Variables
    Scene* scene_context_;
    Setting setting_;
    Entity* selected_entity_;
  };
  
} // namespace ikan
