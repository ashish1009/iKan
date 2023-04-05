//
//  property_grid.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "property_grid.hpp"

namespace ikan {
  
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
