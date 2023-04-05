//
//  Imgui_api.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
  
#define CHECK_WIDGET_FLAG(x) RETURN_IF(x and *x == false);
  
  /// Imgui Wrapper class
  class ImguiAPI {
  public:
    /// This function Enables the docking in Imgui. To be called each frame before every imgui rendering;
    static void StartDcocking();
    /// This function Disable the docking in Imgui. To be called each frame after every imgui rendering;
    static void EndDcocking();

    /// This function renders a widgit that shows the frame rate of the application
    /// - Parameter is_open: flag to show or hide the widget
    static void Framerate(bool *is_open);

    template<typename UIFunction>
    /// This function wraps the Imgui Begin Menu
    /// - Parameters:
    ///   - menu_title: Menu title
    ///   - enable: Is menu enabled
    ///   - func: function to called when menu clicked
    static void Menu(const std::string& menu_title, bool enable, UIFunction func) {
      if (ImGui::BeginMenu(menu_title.c_str(), enable)) {
        func();
        ImGui::EndMenu();
      }
    }
    
    template<typename UIFunction>
    /// This function wraps the Imgui Menu Item
    /// - Parameters:
    ///   - menu_title: Menu Item title
    ///   - shortcut: Shortcut to printed
    ///   - selected: is Item selected
    ///   - enable: is Item enabled
    ///   - func: functon to called when menue item is clicked
    static void MenuItem(const std::string& menu_title, const char* shortcut, bool selected, bool enable, UIFunction func)  {
      if (ImGui::MenuItem(menu_title.c_str(), shortcut, selected, enable)) {
        func();
      }
    }

    MAKE_PURE_STATIC(ImguiAPI);
  };
  
}
