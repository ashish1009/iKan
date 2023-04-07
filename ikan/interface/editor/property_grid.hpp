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
    /// This function render a hint message when hovered last imgui item
    /// - Parameters:
    ///   - description: String to be used as hint
    ///   - error: flag to render text in red
    static void HoveredMsg(const char* description, bool error = false);
    /// This function render a hint icon (?) and on hovered A hint string will be shown in subwindow
    /// on hovering the mouse
    /// - Parameters:
    ///   - description: String to be used as hint
    ///   - icon_char: Icon to be printed as Help Marker (By default (?) will be rendered)
    ///   - same_line: check for same line icon or in new line
    static void HelpMarker(const char* description, const char* icon_char = "?", bool same_line = true);

    /// This fucntion renders the CheckBox to select item
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - column_width_2: width of column of lable
    static bool CheckBox(const char* label, bool& value,
                         float column_width_1 = ImGui::GetWindowContentRegionMax().x / 2);

    /// This function renderes a image button
    /// - Parameters:
    ///   - lableId: image lable string
    ///   - texId: texture id
    ///   - size: size of image
    ///   - bgColor: background color
    ///   - padding: padding
    static bool ImageButton(const std::string& lable_id, uint32_t texId, const glm::vec2& size,
                            const glm::vec4& bgColor = glm::vec4(0.0f), int32_t padding = 0);

    /// This function renders slider for float
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float1(const char* label, float& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = 0.0f, float max_value = FLT_MAX, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float2(const char* label, glm::vec2& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = 0.0f, float max_value = FLT_MAX, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float3(const char* label, glm::vec3& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = 0.0f, float max_value = FLT_MAX, float column_width = ImGui::GetWindowContentRegionMax().x / 2);

  private:
    static bool FloatImpl(const std::vector<std::string>& buttons, const char* label, const std::vector<float*>& values,
                          bool* checkbox_flag, float delta, float reset_value, float min_value, float max_value, float column_width);
  };
  
} // namespace ikan
