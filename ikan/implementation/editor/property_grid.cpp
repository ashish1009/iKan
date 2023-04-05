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
  
} // namespace ikan
