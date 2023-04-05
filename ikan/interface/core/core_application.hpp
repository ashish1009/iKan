//
//  core_application.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/window.hpp"
#include "core/layer_stack.hpp"
#include "core/events/application_event.h"
#include "renderer/utils/renderer.hpp"
#include "renderer/utils/renderer.hpp"
#include "imgui/imgui_layer.hpp"

namespace ikan {
  
  class Application;
  
  /// Defination API for creating application instance.
  /// - Note: only defination is defined in core. Declaration should be at client side based on the application they want to create
  std::unique_ptr<Application> CreateApplication();
  
  /// This class is the base class of any application that will be running in the client side. This class is responsible for
  ///   - Creating Window for the current supported OS
  ///   - Initialize the renderer for the current renderer API
  ///   - Add and store Layer for rendering or any other operations
  ///   - Render the GUI window
  ///   - Handle all the events of the windows Idly events should be handled at Window class only but functionlity depends on the application
  ///     (passed as function pointer)
  class Application {
  public:
    /// This structure holds the specification of application provided by client
    struct Specification {
      std::string name = "iKan";
      std::string workspace_path;
      std::string client_asset_path;
      std::string save_ini_file_path;
      Renderer::Api rendering_api = Renderer::Api::None;
      OperatingSystem os = OperatingSystem::None;
      Window::Specification window_specification;
      bool start_maximized = true;
      bool resizable = true;
      
      void Log();
    };
    
    /// This constructs the core application instance
    /// - Parameter spec: application_specification
    Application(const Specification& spec);
    /// This destroys the core application instance
    virtual ~Application();
    
    // NOTE: Override these virtual Methods in client Application only if you want to create complete fresh application and add some specialisation
    // functionality. If these methods will be overriden in client side then functionality will be completely based on overriden methods
    /// This function runs the game loop of the application. This function is responsible for:
    ///   - Updating the Application.:
    ///   - Updating each Layer frame.:
    ///   - Updating the window and Swap the buffers:
    ///   - Render the GUI by calling 'RenderGui()':
    /// - Important: This API performs all the above actions for each API. In Core ikan::entry_point this funciton is called by default
    virtual void Run();
    /// This Function responsible for closing the current applciation (ikan::Applciation) adn ends the game loop
    virtual void Close() { is_running_ = false; }

    /// This function handles all the events of window. Application is dispatching the events and perform the interupt actions
    /// - Parameter event: event abstract type
    void HandleEvents(Event& event);
    /// This function maximise the window
    void MaximizeWindow() const { window_->Maximize(); }
    /// This function Push the layer of type ikan::Layer in Core Application layer stack. Also attach the layer (initialise it)
    /// - Parameter layer: Layer Reference pointer to be added
    void PushLayer(const std::shared_ptr<Layer>& layer) { layer_stack_.PushLayer(layer); }
    /// This function Pop the layer of type ikan::Layer from Core Application layer stack. Also detach the layer (destroy it)
    /// - Parameter layer: Layer Reference pointer to be removed
    void PopLayer(const std::shared_ptr<Layer>& layer) { layer_stack_.PopLayer(layer); }

    /// This fuinction returns the native pointer Window native as void
    void* GetWindowPtr() const { return (void*)window_->GetNativeWindow(); }
    /// This fuinction returns the iKan Window Instance as reference
    const Window& GetWindow() const { return *(window_.get());}
    /// This fuinction returns the Imgui Layer pointer Reference
    ImguiLayer& GetImGuiLayer() const { return *(imgui_layer_.get()); }
    /// This function returns the specification of application
    const Specification& GetSpecification() const { return specification_; }
    
    /// This fuinction returns the reference instance of application
    static Application& Get() { return *instance_; }
    
    /// Make Application singleton
    DELETE_COPY_MOVE_CONSTRUCTORS(Application);
    
  private:
    /// This function dispatched in event dispatcher and trigger when window close event evoked
    /// - Parameter window_close_event: Window close event instacnce
    bool WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event);
    /// This function begin the Imgui Renderer and render IMGUI for all the layers and finally ends the imgui rendering
    void RenderGui();
    /// This function logs the application configurations
    void LogConfig();

    Application::Specification specification_;
    LayerStack layer_stack_;
    Timestep time_step_;
    std::shared_ptr<Window> window_;
    std::shared_ptr<ImguiLayer> imgui_layer_;
    bool is_running_ = true;
    
    /// Static instance of singleton Application
    static Application *instance_;
  };
    
} // namespace ikan
