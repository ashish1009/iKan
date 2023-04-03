//
//  mac_window.hpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/window.hpp"
#include "renderer/graphics/graphics_context.hpp"

namespace ikan {
  
  /// This class is responsble for creating Window for MAC OS using GLFW
  class MacWindow : public Window {
  public:
    /// Mac window Data
    struct Data {
      Window::Specification specification;
      EventCallbackFn event_callback_function;
    };

    /// This constructuor creates an GLFW window for MAC OS. Register the data in window set back pointer
    /// - parameter specification: window specification
    MacWindow(const Specification& window_spec);
    /// Destroy the GLFW window and shutdown.
    ~MacWindow();
    
    /// This function updates the window for each frame. This function to be called by application on each frame. It swap the context buffers.
    /// Also update the time of each frame
    void Update() override;
    /// This funtion shut down the current window and destroy the GLFW Context
    void Shutdown() override;
    /// This function maximise the window to its full length based on the current monitor. Call when Window need to be maximised
    void Maximize() override;
    /// This function places the window at the center of the screen
    void CenterWindow() override;
    /// This function Updates the callback funtion pointer to the mac_window_data_. This function pointer to be triggered for every event interrupt
    /// - Parameter event_callback_fn: function pointer to be registered
    /// - Important: EventCallbackFn to be called every time any even triggered to window
    void SetEventFunction(const EventCallbackFn& event_callback_fn) override { mac_window_data_.event_callback_function = event_callback_fn; }

    /// This funtions changes the VSync flag by argiument flag
    /// - Parameter vSync: new flag to be set as v sync
    void SetVSync(bool vSync) override;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API
    /// - Parameter resizable: new flag as resizable
    void SetResizable(bool resizable) const override;
    /// This function chnages the title of the window as 'new_title'
    /// - Parameter new_title: new title of the window
    void SetTitle(const std::string& new_title) override;
    
    /// This function returns is window is v synched
    bool IsVSync() const override { return mac_window_data_.specification.v_sync; }
    /// This function returns is window is decoarated. When decorated then removes titlebar from the window
    bool IsTitleBarHidden() const override { return mac_window_data_.specification.hide_titlebar; }
    /// This function returns is the window is full screen
    bool IsFullscreen() const override { return mac_window_data_.specification.fullscreen; }
    /// This function returns the width of window
    uint32_t GetWidth() const override { return mac_window_data_.specification.width; }
    /// This function returns the height of window
    uint32_t GetHeight() const override { return mac_window_data_.specification.height; }
    /// This function returns the title of the window
    std::string GetTitle() const override { return mac_window_data_.specification.title; }
    /// This function returns the current time step of this frame
    Timestep GetTimestep() const override { return time_step_; }
    /// This function returns the native window pointer (void*)
    void* GetNativeWindow() const override { return (void*)window_; }
    
  private:
    /// This function sets the event callback fucntions to window data
    void SetEventCallbacks();

    GLFWwindow* window_;
    Data mac_window_data_;
    std::unique_ptr<GraphicsContext> graphics_context_;
    Timestep time_step_;
    float last_frame_time_ = 0.0f;
  };
  
} // namespace ikan
