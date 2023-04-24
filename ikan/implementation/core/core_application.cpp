//
//  core_application.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "core_application.hpp"
#include "renderer/utils/renderer_stats.hpp"
#include "renderer/utils/batch_2d_renderer.hpp"
#include "renderer/utils/text_renderer.hpp"
#include "editor/content_browser_panel.hpp"

namespace ikan {
  
#define APP_INFO(...) IK_CORE_INFO(LogModule::Application, __VA_ARGS__)
  
  namespace applicaiton_utils {
    
#ifdef IK_DEBUG_FEATURE
    /// This function returns the Operating System Type as string
    /// - Parameter os: OS Type in iKan Enum
    static std::string GetOsNameAsString(OperatingSystem os) {
      switch (os) {
        case OperatingSystem::Mac : return "MAC OS";
        case OperatingSystem::None :
        default:
          IK_ASSERT(false, "Invalid OS Type");
      }
    }
#endif
    
  } // namespace applicaiton_utils

  Application *Application::instance_ = nullptr;
  
  Application::Application(const Specification& spec)
  : specification_(spec) {
    // If instance already created then abort the application as multiple instacnes should not be therer
    IK_CORE_ASSERT(!instance_, "Application already exists !!!");
    instance_ = this;
    
    IK_CORE_TRACE(LogModule::Application, "Creating Core Application Instance ...");

    // Create Memroy for Renderer Data
    // NOTE: Creating the Renderer Data Memory in very begining as this will setup the Renderer API to be used to create any Renderer Implementation
    Renderer::CreateRendererData(specification_.rendering_api);

    // Set the Common direcotry paths
    DirectoryManager::client_asset_path_ = specification_.client_asset_path;
    DirectoryManager::workspace_path_ = specification_.workspace_path;

    // Create window instance
    window_ = Window::Create(specification_.os, specification_.window_specification);
    
    // Set the application callback to window
    window_->SetEventFunction(IK_BIND_EVENT_FN(Application::HandleEvents));
    
    // Decorate Window
    window_->SetResizable(specification_.resizable);
    if (specification_.start_maximized)
      window_->Maximize();
    
    // Initialize the Imgui Layer if GUI is enabled
    imgui_layer_ = std::make_shared<ImguiLayer>(window_);
    layer_stack_.PushOverlay(imgui_layer_);
    imgui_layer_->SetIniFilePath(specification_.save_ini_file_path);
    
    // Create Memroy for Renderer Data
    Renderer::Initialize();
  }

  Application::~Application() {
    IK_CORE_TRACE(LogModule::Application, "Destroying Core Application Instance !!!");
    Renderer::Shutdown();
    ContentBrowserPanel::Shutdown();
  }

  void Application::HandleEvents(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
    
    // Event Handler for all layers
    for (auto& layer : layer_stack_)
      layer->HandleEvents(event);
    
    // Client side event handler funciton
    OnEventHander(event);
  }
  
  bool Application::WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event) {
    Close();
    return false;
  }

  void Application::Run() {
    // Log the Configuration of the Engine
    LogConfig();
    Batch2DRenderer::LogData();
    TextRenderer::LogData();
    
    // Client side Init funciton
    OnInitialise();

    IK_CORE_INFO("", "--------------------------------------------------------------------------");
    IK_CORE_INFO("", "                          Starting Game Loop                              ");
    IK_CORE_INFO("", "--------------------------------------------------------------------------");
    while(is_running_) {
      // Store the frame time difference
      time_step_ = window_->GetTimestep();
      
      // Window update each frame
      window_->Update();
      
      // Reset Statistics each frame
      Renderer::ResetStatsEachFrame();
      
      // Updating all the attached layer
      for (auto& layer : layer_stack_)
        layer->Update(time_step_);
      
      // Client side update funciton
      OnUpdate(time_step_);
      
      RenderGui();
    }
    IK_CORE_INFO("", "--------------------------------------------------------------------------");
    IK_CORE_INFO("", "                            Ending Game Loop                              ");
    IK_CORE_INFO("", "--------------------------------------------------------------------------");
    
    // Client side shutdown funciton
    OnShutdown();
  }
  
  void Application::RenderGui() {
    imgui_layer_->Begin();
    
    // Updating all the attached layer
    for (auto& layer : layer_stack_)
      layer->RenderGui();
    
    // Client side Gui Renderer funciton
    OnRenderGui();
    
    imgui_layer_->End();
  }
  
  void Application::LogConfig() {
    APP_INFO("Loading the {0} application ...", specification_.name);
    
    APP_INFO("    Application                           | {0}", specification_.name);
    APP_INFO("        Operating System                  | {0}", applicaiton_utils::GetOsNameAsString(specification_.os));
    APP_INFO("        Rewndering API                    | {0}", renderer_utils::GetRendererApiName(specification_.rendering_api));
    APP_INFO("            Vendor                        | {0} ", Renderer::Capabilities::Get().vendor);
    APP_INFO("            Renderer                      | {0} ", Renderer::Capabilities::Get().renderer);
    APP_INFO("            Version                       | {0} ", Renderer::Capabilities::Get().version);
    APP_INFO("            Asignment                     | Unpacked");
    APP_INFO("            Multi Sample Feild            | true");
    APP_INFO("            Blending Test Feild           | true");
    APP_INFO("            Depth Test Feild              | true");

    APP_INFO("        Window                            | {0}", specification_.window_specification.title);
    APP_INFO("            Width                         | {0}", specification_.window_specification.width);
    APP_INFO("            Height                        | {0}", specification_.window_specification.height);
    APP_INFO("            Video Synced                  | {0}", specification_.window_specification.v_sync);
    APP_INFO("            FullScreen                    | {0}", specification_.window_specification.fullscreen);
    APP_INFO("            Title Hidden                  | {0}", specification_.window_specification.hide_titlebar);
    APP_INFO("            Maximized                     | {0}", specification_.start_maximized);
    APP_INFO("            Resizable                     | {0}", specification_.resizable);
    APP_INFO("        Directories                       | {0}", specification_.resizable);
    APP_INFO("            Workspace Path                | {0}", specification_.workspace_path.c_str());
    APP_INFO("            Client Asset Path             | {0}", specification_.client_asset_path.c_str());
    APP_INFO("            Save Gui ini File Path        | {0}", specification_.save_ini_file_path.c_str());
  }

} // namespace ikan
