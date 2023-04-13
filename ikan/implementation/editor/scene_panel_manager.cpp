//
//  scene_panel_manager.cpp
//  ikan
//
//  Created by Ashish . on 12/04/23.
//

#include "scene_panel_manager.hpp"
#include "scene.hpp"
#include "property_grid.hpp"

namespace ikan {
  
  ScenePanelManager::ScenePanelManager(Scene* context) : scene_context_(context) {
    IK_CORE_TRACE(LogModule::ScenePanelManager, "Creating Scene pannel Manager ...");
  }
  
  ScenePanelManager::~ScenePanelManager() {
    IK_CORE_TRACE(LogModule::ScenePanelManager, "Destroying Scene pannel Manager !!!");
  }
  
  void ScenePanelManager::RenderGui() {
    ScenePannel();
    PropertyPannel();
  }
  
  void ScenePanelManager::ScenePannel() {
    if (!setting_.scene_panel)
      return;
    
    ImGui::Begin("Scene Manager", &setting_.scene_panel);
    ImGui::PushID("Scene Manager");
    
    ImGui::Text(" Scene | %s ", scene_context_->name_.c_str());
    std::string hovered_msg =
    "Num Entities                  : " + std::to_string(scene_context_->num_entities_)  + "\n" +
    "Max Entity ID                 : " + std::to_string(scene_context_->max_entity_id_) + "\n" +
    "Registry Max Capacity : " + std::to_string(scene_context_->curr_registry_capacity) + "\n" +
    "Registry Size                  : " + std::to_string(scene_context_->registry_.size()) + "\n";
    PropertyGrid::HoveredMsg(hovered_msg.c_str());

    ImGui::PopID();
    ImGui::End();
  }
  
  void ScenePanelManager::PropertyPannel() {
    if (!setting_.property_panel)
      return;
    
    ImGui::Begin("Entity Property", &setting_.property_panel);
    ImGui::PushID("Entity Property");
    
    ImGui::PopID();
    ImGui::End();
  }
  
} // namespace ikan
