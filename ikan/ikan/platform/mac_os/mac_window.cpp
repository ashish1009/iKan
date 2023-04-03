//
//  mac_window.cpp
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#include "mac_window.hpp"
#include "core/events/application_event.h"
#include "core/events/mouse_event.h"
#include "core/events/key_event.h"

namespace ikan {
  
  MacWindow::MacWindow(const Specification& window_spec) {
    IK_CORE_INFO(LogModule::Window, "Creating MAC OS Window instacne ... ");
    
    // Move the specificaion to MacWindow data
    mac_window_data_.specification = window_spec;
    mac_window_data_.specification.Log();
    
    // Initialize the library
    IK_ASSERT(GLFW_TRUE == glfwInit(), "Can not Initialize GLFW Window");
    
    // Configure GLFW Context Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // This removes titlebar on all platforms and all of the native window effects on non-Windows platforms
    glfwWindowHint(GLFW_DECORATED, mac_window_data_.specification.hide_titlebar);
    
    // Params to create GLFW window
    int32_t width = (int32_t)mac_window_data_.specification.width;
    int32_t height = (int32_t)mac_window_data_.specification.height;
    GLFWmonitor* primary_monitor = nullptr;
    GLFWwindow* share_monitor = nullptr;
    
    // Overwrite the data to create full screen window
    if (mac_window_data_.specification.fullscreen) {
      const GLFWvidmode* mode = glfwGetVideoMode(primary_monitor);
      width = mode->width;
      height = mode->height;
      primary_monitor = glfwGetPrimaryMonitor();
    }
    window_ = glfwCreateWindow(width, height, mac_window_data_.specification.title.c_str(), primary_monitor, share_monitor);

    // If Window is not created successfully then terminate...
    if (window_ == NULL) {
      glfwTerminate();
      IK_CORE_ASSERT(false, "Unable to create Window!!!!");
    }
    
    // Setting VSync as True
    glfwSwapInterval(mac_window_data_.specification.v_sync);

    // Set the User defined pointer to GLFW Window, this pointer will be retrieved when an interrupt will be triggered
    glfwSetWindowUserPointer(window_, &mac_window_data_);

    // Set GLFW Callbacks
    SetEventCallbacks();
    
    graphics_context_ = GraphicsContext::Create(window_);
  }
  
  MacWindow::~MacWindow() {
    IK_CORE_WARN(LogModule::Window, "Destroying MAC OS Window instacne !!! ");
    Shutdown();
  }
  
  void MacWindow::SetEventCallbacks() {
    // -------------- Call back for Window Resize Event
    glfwSetWindowSizeCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t width,
   [[maybe_unused]] int32_t height
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     data.specification.width  = (uint32_t)width;
     data.specification.height = (uint32_t)height;
     
     WindowResizeEvent event(data.specification.width, data.specification.height);
     data.event_callback_function(event);
   });
    
    // -------------- Call back for Window Close Event
    glfwSetWindowCloseCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     WindowCloseEvent event;
     data.event_callback_function(event);
   });
    
    // -------------- Call back for Window Focused Event
    glfwSetWindowFocusCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t icontified) {
   });
    
    // -------------- Call back for Set the Key Event
    glfwSetKeyCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t key,
   [[maybe_unused]] int32_t scan_code,
   [[maybe_unused]] int32_t action,
   [[maybe_unused]] int32_t mods
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     
     switch (action) {
       case GLFW_PRESS: {
         KeyPressedEvent event(static_cast<Key>(key), 0);
         data.event_callback_function(event);
         break;
       }
       case GLFW_RELEASE: {
         KeyReleasedEvent event(static_cast<Key>(key));
         data.event_callback_function(event);
         break;
       }
       case GLFW_REPEAT: {
         KeyPressedEvent event(static_cast<Key>(key), 1);
         data.event_callback_function(event);
         break;
       }
     }
   });
    
    // -------------- Call back for Character pressed Event
    glfwSetCharCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] uint32_t key_code
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     KeyTypedEvent event(static_cast<Key>(key_code));
     data.event_callback_function(event);
   });
    
    // -------------- Call back for Mouse button pressed Event
    glfwSetMouseButtonCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t button,
   [[maybe_unused]] int32_t action,
   [[maybe_unused]] int32_t mods
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     
     switch (action) {
       case GLFW_PRESS: {
         MouseButtonPressedEvent event(static_cast<MouseButton>(button));
         data.event_callback_function(event);
         break;
       }
       case GLFW_RELEASE: {
         MouseButtonReleasedEvent event(static_cast<MouseButton>(button));
         data.event_callback_function(event);
         break;
       }
     }
   });
    
    // -------------- Call back for Mouse Scrolled Event
    glfwSetScrollCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] double x_offset,
   [[maybe_unused]] double y_offset
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     MouseScrolledEvent event((float)x_offset, (float)y_offset);
     data.event_callback_function(event);
   });
    
    // -------------- Call back for Mouse Cursor Event
    glfwSetCursorPosCallback( window_,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] double x_position,
   [[maybe_unused]] double y_position
   ) {
     MacWindow::Data& data = *(MacWindow::Data*)glfwGetWindowUserPointer(window);
     MouseMovedEvent event((float)x_position, (float)y_position);
     data.event_callback_function(event);
   });
    
  }
  
  void MacWindow::Shutdown() {
    IK_CORE_WARN(LogModule::Window, "Shutting down the MAC OS Window");
    glfwTerminate();
    glfwDestroyWindow(window_);
  }
  
  void MacWindow::Update() {
    graphics_context_->SwapBuffers();
    glfwPollEvents();
    
    float current_frame_time = static_cast<float>(glfwGetTime());
    time_step_ = current_frame_time - last_frame_time_;
    last_frame_time_ = current_frame_time;
  }

  void MacWindow::Maximize() {
    IK_CORE_INFO(LogModule::Window, "Maximising the window");
    glfwMaximizeWindow(window_);
  }
  
  void MacWindow::CenterWindow() {
    IK_CORE_INFO(LogModule::Window, "Placing the window at the center");
    const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    const Window::Specification& spec = mac_window_data_.specification;
    int32_t x = (videmode->width / 2) - ((int32_t)spec.width / 2);
    int32_t y = (videmode->height / 2) - ((int32_t)spec.height / 2);
    glfwSetWindowPos(window_, x, y);
  }
  
  void MacWindow::SetVSync(bool enabled) {
    IK_CORE_INFO(LogModule::Window, "Setting VSynch : {0}", enabled);
    (true == enabled) ? glfwSwapInterval(1) : glfwSwapInterval(0);
    mac_window_data_.specification.v_sync = enabled;
  }
  
  void MacWindow::SetResizable(bool resizable) const {
    std::string can_cannot_string = resizable ? "can" : "cannot";
    IK_CORE_INFO(LogModule::Window, "Window {0} be resized", can_cannot_string.c_str());
    glfwSetWindowAttrib(window_, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE );
  }
  
  void MacWindow::SetTitle(const std::string& title) {
    IK_CORE_INFO(LogModule::Window, "New MAC Window Title is : {0}", title.c_str());
    IK_CORE_WARN(LogModule::Window, "(WARNING: Window specificaiton instance in Application Spceification might have older Window name..)");
    
    mac_window_data_.specification.title = title;
    glfwSetWindowTitle(window_, mac_window_data_.specification.title.c_str());
  }
  
  bool MacWindow::IsActive() const {
    return !glfwWindowShouldClose(window_);
  }

} // namespace ikan
