//
//  imgui_layer.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/layer.hpp"

namespace ikan {
  
  class Window;
    
  /// This class creates and manages the Imgui Layer in application
  /// - Note: If Not using iKan Application then create instance of Imgui Layer in Client Application and manage API Accordingly, by default
  ///         Application is taking care of all
  class ImguiLayer : public Layer {
  public:
    /// This constructor creates the Imgui layer instance.
    /// - Parameter window_pointer: application window pointer
    ImguiLayer(std::shared_ptr<Window> window);
    /// This destrictor destroy the Imgui layer instance
    ~ImguiLayer() noexcept;
    
    /// This funciton initialises the imgui layer. Creates the imgui layer using imgui file library
    void Attach() override;
    /// This funciton destroy the imgui layer delete all the imgui created impl
    void Detach() override;
    /// This function do nothing for now
    void RenderGui() override {}
    /// This function do nothing for now
    void Update([[maybe_unused]] Timestep ts) override;
    /// This function handles all the events triggered for the imgui layer. To be called whenever an event is triggered in the window
    /// - Note: Core ikan::Application is taking care to call the HandleEvents(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void HandleEvents(Event& event) override;
    
    /// This function begins the imgui rendering for each frame NOTE: Call once per frame
    void Begin();
    /// This function ends the rendering of imgui each frame NOTE: Call once per frame
    void End();
    /// This function the save location of ini file path
    /// - Parameter ini_file_path: file path relative to executable
    /// - Note: Directory must exist.
    void SetIniFilePath(const std::string& ini_file_path);
    /// This function blocks the Imgui Events if flag is true
    /// - Parameter block: block flag
    void BlockEvents(bool block);

    DELETE_COPY_MOVE_CONSTRUCTORS(ImguiLayer);
    
  private:
    bool block_events_ = true;
    std::shared_ptr<Window> window_ = nullptr;
  };
  
} // namespace ikan
