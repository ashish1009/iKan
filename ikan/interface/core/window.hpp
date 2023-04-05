//
//  window.hpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/events/event.h"
#include "core/utils/time_step.h"

namespace ikan {
  
  /// This class is the abstract class for creating Window instance. This class defines all the API to be used by core or client.
  /// - Note: To Create instance of window Call the static API 'Window::Create' with Operating system type and specificaion of windows
  class Window {
  public:
    /// This structure holds the specification for window.
    struct Specification {
      std::string title = "ikan Window";
      uint32_t width = 1600, height = 900;

      bool v_sync = true;
      bool fullscreen = false;
      bool hide_titlebar = false;

      void Log();
    };

    /// This function create instance of to create a window based on the Operating system supported by the sytem / Engine. This is resposible to create
    /// the GLFW Window instance for the OS and Create the Renderer Context for the suppoorted Renderer API.
    /// - Parameters:
    ///   - os: Current operating system
    ///   - specificaiton: window specification
    /// - Important: Set the renderer API befiore creating the Widnow, as it is needed to creaste the Contexrt
    static std::unique_ptr<Window> Create(OperatingSystem os, const Specification& spec);

    /// Default virtual destructor
    virtual ~Window() noexcept = default;
    
    /// This function updates the window for each frame. This function to be called by application on each frame. It swap the context buffers.
    /// Also update the time of each frame
    virtual void Update() = 0;
    /// This funtion shut down the current window and destroy the GLFW Context
    virtual void Shutdown() = 0;
    /// This function updates the event callback function and register it to the window instance
    /// - Parameter event_callback_fn: function pointer to be registered in the window
    /// - Important: EventCallbackFn to be called every time any even triggered to window
    virtual void SetEventFunction(const EventCallbackFn& event_callback_fn) = 0;
    /// This function maximise the window to its full length based on the current monitor. Call when Window need to be maximised
    virtual void Maximize() = 0;
    /// This function places the window at the center of the screen
    virtual void CenterWindow() = 0;
    
    /// This funtions changes the VSync flag by argiument flag
    /// - Parameter vSync: new flag to be set as v sync
    virtual void SetVSync(bool vSync) = 0;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API
    /// - Parameter resizable: new flag as resizable
    virtual void SetResizable(bool resizable) const = 0;
    /// This function chnages the title of the window as 'new_title'
    /// - Parameter new_title: new title of the window
    virtual void SetTitle(const std::string& new_title) = 0;
    
    /// This function returns is window is active
    virtual bool IsActive() const = 0;
    /// This function returns is window is v synched
    virtual bool IsVSync() const = 0;
    /// This function returns is window is decoarated. When decorated then removes titlebar from the window
    virtual bool IsTitleBarHidden() const = 0;
    /// This function returns is the window is full screen
    virtual bool IsFullscreen() const = 0;
    /// This function returns the width of window
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the height of window
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the current time step of this frame
    virtual Timestep GetTimestep() const = 0;
    /// This function returns the native window pointer (void*)
    virtual void* GetNativeWindow() const = 0;
    /// This function returns the title of the window
    virtual std::string GetTitle() const = 0;
  };
  
} // namespace ikan
