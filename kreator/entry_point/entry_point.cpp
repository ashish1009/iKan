//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include <iostream>


#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <examples/imgui_impl_opengl3.cpp>
#include <examples/imgui_impl_glfw.cpp>

#include <GLFW/glfw3.h>

void EventHandler(ikan::Event& event) {
  
}

int main() {
  // Initialize the ikan Logger
  auto core_level   = ikan::Logger::Level::Trace;
  auto client_level = ikan::Logger::Level::Trace;
  
  ikan::Logger::Init(core_level, client_level, "../../../log/kreator.log");
  
#ifdef IK_DEBUG_FEATURE
  auto spd_core_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(core_level);
  auto spd_client_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(client_level);
  
  IK_INFO("Core Entry Point", "Initialized the spd logger ");
  IK_INFO("Core Entry Point", "  Core   | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_core_log_level));
  IK_INFO("Core Entry Point", "  Client | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_client_log_level));
#endif
  
  ikan::Window::Specification window_specification;
  
  window_specification.title = "Untitled Window";
  window_specification.width = 2100;
  window_specification.height = 900;
  window_specification.v_sync = true;
  window_specification.fullscreen = false;
  window_specification.hide_titlebar = false;
  
  std::unique_ptr<ikan::Window> window_;
  {
    ikan::Renderer::CreateRendererData(ikan::Renderer::Api::OpenGl);
    
    window_ = ikan::Window::Create(ikan::OperatingSystem::Mac, window_specification);
    window_->SetEventFunction(std::bind(EventHandler, std::placeholders::_1));
    
    ikan::DirectoryManager::SetClientAssetPath("../../../kreator/editor/assets");
    
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
    
    {
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.IniFilename = "../../../kreator/editor/ini/editor.ini";
    }
    
    ikan::Renderer::Initialize();

    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                     Core Application Initialized                         ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }

  {
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                          Starting Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");

    while (window_->IsActive()) {
      window_->Update();
      
      {
        {
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          
          ImGui::SaveIniSettingsToDisk("testing.ini");
          
          ImGui::NewFrame();
        }
        
        {
          ImGui::ShowDemoWindow();
        }
        
        {
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
      }
    }
    
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                            Ending Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }
  {
    ikan::Renderer::Shutdown();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  
  return 0;
}
