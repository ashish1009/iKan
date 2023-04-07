//
//  property_grid.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "property_grid.hpp"

namespace ikan {

  void PropertyGrid::HoveredMsg(const char* description, bool error) {
    if (ImGui::IsItemHovered()) {
      if (error)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
      
      ImGui::BeginTooltip();
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(description);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
      
      if (error)
        ImGui::PopStyleColor();
    }
  }
  
  void PropertyGrid::HelpMarker(const char* description, const char* icon_char, bool same_line) {
    if (same_line)
      ImGui::SameLine();
    
    ImGui::TextDisabled(icon_char);
    HoveredMsg(description);
  }
  
  bool PropertyGrid::CheckBox(const char* label, bool& value, float column_width_1, float column_width_2) {
    bool modified = false;
    
    ImGui::Columns(2);
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(0, column_width_1);
    ImGui::Text(label);
    ImGui::PopItemWidth();
    
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::SetColumnWidth(1, column_width_2);
    std::string UIContextId = "##" + (std::string)label;
    
    if (ImGui::Checkbox(UIContextId.c_str(), &value))
      modified = true;
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    return modified;
  }

  bool PropertyGrid::ImageButton(const std::string& lable_id, uint32_t texId, const glm::vec2& size, const glm::vec4& bgColor, int32_t padding) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    
    auto& colors = ImGui::GetStyle().Colors;
    const auto& button_hovered = colors[ImGuiCol_ButtonHovered];
    
    // No Background color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 0.5f));
    const auto& button_cctive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(button_cctive.x, button_cctive.y, button_cctive.z, 0.5f));
    
    bool result = false;
    ImTextureID my_texture_id = (ImTextureID)((size_t)texId);
    ImGui::PushID(lable_id.c_str());
    result = ImGui::ImageButton(my_texture_id, { size.x, size.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), padding,
                                { bgColor.r, bgColor.g, bgColor.b, bgColor.a });
    ImGui::PopID();
    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    
    return result;
  }
  
} // namespace ikan
