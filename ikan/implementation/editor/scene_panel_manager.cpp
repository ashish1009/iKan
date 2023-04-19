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
#include "imgui_api.hpp"
#include "renderer.hpp"
#include "texture.hpp"

namespace ikan {
  
#define HAS_COMPONENT(Component) selected_entity_->HasComponent<Component>()
  
  /// This function renders the components in property pannel. Takes the function pointer in argument
  /// - Parameters:
  ///   - name: name of the entity
  ///   - entity: entity reference pointer
  ///   - ui_function: ui function to be called
  template<typename T, typename UIFunction> static void DrawComponent(const std::string& name, const Entity& entity, UIFunction ui_function) {
    // Flag for rendering the title of entity
    const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                               ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    
    // Render the property if entity have a component component
    if (entity.HasComponent<T>()) {
      // Title style
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
      
      // Render the title named as entity name
      bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_node_flags, name.c_str());
      ImGui::PopStyleVar(); // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
      
      // Get the avilable width and height for button position
      ImVec2 content_region_available = ImGui::GetContentRegionAvail();
      float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
      
      // Set the curson position on same line for (X) button
      ImGui::SameLine(content_region_available.x - line_height * 0.5f);
      float content_height = GImGui->Font->FontSize;
      
      const auto& current_cursor_pos = ImGui::GetCursorPos();
      ImGui::SetCursorPos({current_cursor_pos.x, current_cursor_pos.y + content_height / 4});
      
      // Render the button (X) for removing the component
      static std::shared_ptr<Texture> close_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/gear.png"));
      if (PropertyGrid::ImageButton("Close", close_texture->GetRendererID(), { content_height, content_height } )) {
        ImGui::OpenPopup("ComponentSettings");
      }
      
      // Pop up for removing the component
      bool remove_component = false;
      if (ImGui::BeginPopup("ComponentSettings")) {
        if (ImGui::MenuItem("Remove component"))
          remove_component = true;
        
        ImGui::EndPopup();
      }
      
      // Call the function pointer
      if (open) {
        auto& component = entity.GetComponent<T>();
        ui_function(component);
        ImGui::TreePop();
      }
      
      // Remove the component if button clicked
      if (remove_component)
        entity.RemoveComponent<T>();
    } // if (entity.HasComponent<T>())
  }
  
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

    // Reset the selected entity if mouse is clicked on empty space
    if (ImGui::IsMouseDown((int32_t)MouseButton::ButtonLeft) and ImGui::IsWindowHovered()) {
      SetSelectedEntity(nullptr);
    }

    // Menu popup on Right Click
    if (ImGui::BeginPopupContextWindow(0 /* ID */, (int32_t)MouseButton::ButtonRight, false /* Right-click on blank space */)) {
      RightClickOptions();
      ImGui::EndPopup();
    }

    // Delete the entity
    if (delete_entity_) {
      // Delete entity from scene
      scene_context_->DestroyEntity(*selected_entity_);
      
      SetSelectedEntity(nullptr);
      delete_entity_ = false;
    }

    ImGui::PopID();
    ImGui::End();
  }
  
  void ScenePanelManager::PropertyPannel() {
    if (!setting_.property_panel)
      return;
    
    ImGui::Begin("Entity Property", &setting_.property_panel);
    ImGui::PushID("Entity Property");
    
    if (selected_entity_) {
      auto& tag = selected_entity_->GetComponent<TagComponent>().tag;
      PropertyGrid::TextBox(tag, "", 3);
      PropertyGrid::HoveredMsg(("Entity ID : " + std::to_string((uint32_t)(*selected_entity_))).c_str());

      // Add Component Icon
      // NOTE: we are adjusting this with text box, this would be next column of text box
      auto text_box_size = ImGui::GetItemRectSize();
      ImGui::NextColumn();
      ImGui::SetColumnWidth(2, 2 * text_box_size.y);

      static std::shared_ptr<Texture> add_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/plus.png"));
      if (PropertyGrid::ImageButton("Add", add_texture->GetRendererID(), { text_box_size.y, text_box_size.y })) {
        ImGui::OpenPopup("AddComponent");
      }

      if (ImGui::BeginPopup("AddComponent")) {
        AddComponent();
        ImGui::EndPopup();
      }
      ImGui::Columns(1);
      ImGui::Separator();
      
      // Draw other components
      DrawComponent<TransformComponent>("Transform", *selected_entity_, [](auto& tc) { tc.RenderGui(); });
      DrawComponent<CameraComponent>("Camera", *selected_entity_, [](auto& cc) { cc.RenderGui(); });
      DrawComponent<QuadComponent>("Quad", *selected_entity_, [](auto& qc) { qc.RenderGui(); });
      DrawComponent<CircleComponent>("Circle", *selected_entity_, [](auto& cc) { cc.RenderGui(); });
    }

    ImGui::PopID();
    ImGui::End();
  }
  
  void ScenePanelManager::DrawEntityTreeNode(entt::entity entity_id) {
    Entity& entity = scene_context_->entity_id_map_.at(entity_id);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth |
    ((selected_entity_ and *selected_entity_ == entity) ? ImGuiTreeNodeFlags_Selected : 0);
  
    const std::string& tag = entity.GetComponent<TagComponent>().tag;
    bool opened = ImGui::TreeNodeEx((void*)(tag.c_str()), flags, tag.c_str());
    
    // Left Click Feature. Update the selected entity if item is clicked
    if (ImGui::IsItemClicked() or ImGui::IsItemClicked(1))
      SetSelectedEntity(&entity);
    
    // Right click of mouse option
    if (ImGui::BeginPopupContextItem()) {
      // Duplicate Entity
      if (ImGui::MenuItem("Duplicate Entity")) {
        selected_entity_ = &scene_context_->DuplicateEntity(*selected_entity_);
      }
      
      // Delete Entity
      if (ImGui::MenuItem("Delete Entity")) {
        delete_entity_ = true;
      }
      ImGui::EndMenu(); // Add to group
    }
    
    if (opened) {
      // TODO: Add Feature
      ImGui::TreePop();
    }
  }

  void ScenePanelManager::RightClickOptions() {
    ImguiAPI::Menu("New Entity", true, [this](){
      if (ImGui::MenuItem("Empty Entity")) {
        SetSelectedEntity(&scene_context_->CreateEntity("Empty Entity"));
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Camera")) {
        SetSelectedEntity(&scene_context_->CreateEntity("Camera"));
        selected_entity_->AddComponent<CameraComponent>();
      }

      if (scene_context_->GetType() == Scene::_2D) {
        ImGui::Separator();
        ImguiAPI::Menu("2D Entity", true, [this](){
          if (ImGui::MenuItem("Quad")) {
            SetSelectedEntity(&scene_context_->CreateEntity("Quad"));
            selected_entity_->AddComponent<QuadComponent>();
          }
          if (ImGui::MenuItem("Circle")) {
            SetSelectedEntity(&scene_context_->CreateEntity("Circle"));
            selected_entity_->AddComponent<CircleComponent>();
          }
        });
      }
    });
  }
  
  void ScenePanelManager::AddComponent() {
    if (scene_context_->GetType() == Scene::_2D) {
      AddComponentMenu<QuadComponent>("Camera", !HAS_COMPONENT(CameraComponent));
      ImGui::Separator();
      AddComponentMenu<QuadComponent>("Quad", !HAS_COMPONENT(QuadComponent) and !HAS_COMPONENT(CircleComponent));
      AddComponentMenu<CircleComponent>("Circle", !HAS_COMPONENT(QuadComponent) and !HAS_COMPONENT(CircleComponent));
    }
    else if (scene_context_->GetType() == Scene::_3D) {
    }
  }
  
  void ScenePanelManager::SetSelectedEntity(Entity* entity) {
    if (entity and entity->IsValidScene())
      selected_entity_ = entity;
    else
      selected_entity_ = nullptr;
  }
  
} // namespace ikan
