//
//  property_grid.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include <imgui_internal.h>

namespace ikan {
  
  /// This class stores the Wrapper API for showing Data in Imgui window
  class PropertyGrid {
  public:
    /// This fucntion renders the CheckBox to select item
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - column_width_2: width of column of lable
    ///   - column_width_2: width of column of Value
    static bool CheckBox(const char* label, bool& value,
                         float column_width_1 = ImGui::GetWindowContentRegionMax().x / 2,
                         float column_width_2 = ImGui::GetWindowContentRegionMax().x / 2);

  };
  
} // namespace ikan
