//
//  scene_panel_manager.cpp
//  ikan
//
//  Created by Ashish . on 12/04/23.
//

#include "scene_panel_manager.hpp"
#include "scene.hpp"
#include "components.hpp"
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

    static ImGuiTextFilter entity_filter;
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16);
    if (PropertyGrid::Search(entity_filter.InputBuf, "Search ... "))
      entity_filter.Build();
    
    ImGui::Separator();

    scene_context_->registry_.each([&](auto entity_id)
                                   {
      const std::string& tag = scene_context_->registry_.get<TagComponent>(entity_id).tag;
      // If Search filter pass the result then render the entity name
      if(entity_filter.PassFilter(tag.c_str()))
        DrawEntityTreeNode(entity_id);
    });

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
  
  void ScenePanelManager::DrawEntityTreeNode(entt::entity entity_id) {
    Entity& entity = scene_context_->entity_id_map_.at(entity_id);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ((selected_entity_ == entity) ? ImGuiTreeNodeFlags_Selected : 0);
  
    const std::string& tag = entity.GetComponent<TagComponent>().tag;
    bool opened = ImGui::TreeNodeEx((void*)(tag.c_str()), flags, tag.c_str());
    
    if (opened) {
      // TODO: Add Feature
      ImGui::TreePop();
    }
  }
  
} // namespace ikan
