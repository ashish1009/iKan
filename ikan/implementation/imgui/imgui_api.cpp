//
//  Imgui_api.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "imgui_api.hpp"
#include "editor/property_grid.hpp"
#include "core_application.hpp"

namespace ikan {
  
  void ImguiAPI::StartDcocking() {
    // Note: Switch this to true to enable dockspace
    static bool dockspace_open = false;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into, because it would be confusing to
    // have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    
    if (opt_fullscreen) {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoNavFocus;
    }
    
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole,
    // so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspace_open, window_flags);
    ImGui::PopStyleVar();
    
    if (opt_fullscreen)
      ImGui::PopStyleVar(2);
    
    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
  }
  
  void ImguiAPI::EndDcocking() {
    ImGui::End();
  }
  
  void ImguiAPI::Framerate(bool *is_open) {
    CHECK_WIDGET_FLAG(is_open);
    
    ImGui::Begin("Frame Rate", is_open);
    ImGui::PushID("Frame Rate");
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    PropertyGrid::HoveredMsg(std::to_string((float)1000.0f / (float)ImGui::GetIO().Framerate).c_str());
    ImGui::PopID();
    ImGui::End();
  }
  
  void ImguiAPI::ChangeFont(const Font& default_font_path, const Font& bold_font_path) {
    auto& imgui_layer = Application::Get().GetImGuiLayer();
    imgui_layer.Detach();
    
    imgui_layer.Attach();
    imgui_layer.SetIniFilePath(Application::Get().GetSpecification().save_ini_file_path);
    
    imgui_layer.SetFont(default_font_path, bold_font_path);
  }

} // namespace ikan
