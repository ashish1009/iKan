//
//  imgui_layer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "imgui_layer.hpp"
#include "core/core_application.hpp"
#include "core/events/event.h"

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

namespace ikan {

  ImguiLayer::ImguiLayer(std::shared_ptr<Window> window) : Layer("Imgui Layer"),
  window_(window) {
    IK_CORE_TRACE(LogModule::Imgui, "Creating Imgui Layer instance ...");
  }

  ImguiLayer::~ImguiLayer() noexcept {
    IK_CORE_TRACE(LogModule::Imgui, "Destroying Imgui Layer instance !!!");
  }

  void ImguiLayer::Attach() {
    IK_CORE_TRACE(LogModule::Imgui, "'{0}' is attached to application", GetName());
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    GLFWwindow* window = static_cast<GLFWwindow*>(window_->GetNativeWindow());
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void ImguiLayer::Detach() {
    IK_CORE_TRACE(LogModule::Imgui, "'{0}' is detached to application", GetName());
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  
  void ImguiLayer::HandleEvents(Event& event) {
    if (block_events_) {
      ImGuiIO& io = ImGui::GetIO();
      
      event.handled_ |= event.IsInCategory(Event::EventCategoryMouse) & io.WantCaptureMouse;
      event.handled_ |= event.IsInCategory(Event::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
  }
  
  void ImguiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    
    ImGui::SaveIniSettingsToDisk("testing.ini");
    
    ImGui::NewFrame();
  }
  
  void ImguiLayer::End() {
    ImGuiIO& io      = ImGui::GetIO();
    io.DisplaySize   = ImVec2((float)window_->GetWidth(), (float)window_->GetHeight());
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }

  void ImguiLayer::SetIniFilePath(const std::string& ini_file_path) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = ini_file_path.c_str();
  }
  
  void ImguiLayer::SetFont(const Font &default_font, const Font &bold_font) {
    if (default_font.path == "" or bold_font.path == "")
      return;
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->ClearFonts();
    
    // Store the Bold font as well
    io.Fonts->AddFontFromFileTTF(bold_font.path.c_str(), bold_font.size);
    
    // Default font is Regular
    io.FontDefault = io.Fonts->AddFontFromFileTTF(default_font.path.c_str(), default_font.size);
    
    IK_CORE_TRACE(LogModule::Imgui, "Imgui Font changed");
    IK_CORE_TRACE(LogModule::Imgui, "  Regular Font Path  {0}", default_font.path.c_str());
    IK_CORE_TRACE(LogModule::Imgui, "  Regular Font Size  {0}", default_font.size);
    IK_CORE_TRACE(LogModule::Imgui, "  Bold Font Path     {0}", bold_font.path.c_str());
    IK_CORE_TRACE(LogModule::Imgui, "  Bold Font Size     {0}", bold_font.size);
  }

} // namespace ikan
