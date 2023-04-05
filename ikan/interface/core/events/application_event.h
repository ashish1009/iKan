//
//  application_event.h
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/events/event.h"

namespace ikan {
  
  /// This class handles the event to resize the window
  class WindowResizeEvent : public Event {
  public:
    /// Default Destructor
    virtual ~WindowResizeEvent() = default;
    
    /// Constructs the event that set the new size of window
    /// - Parameters:
    ///  - width: width of new window:
    ///  - height: height of new window:
    WindowResizeEvent(uint32_t width, uint32_t height) : width_(width), height_(height) {}

    /// This function returns the width of window
    uint32_t GetWidth() const { return width_; }
    /// This function returns the height of window
    uint32_t GetHeight() const { return height_; }
    
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Window Resize Event tiggered ");
      IK_TRACE(LogModule::Event, "  Height | {0}", height_);
      IK_TRACE(LogModule::Event, "  Width  | {0}", width_);
    }
    
    EVENT_CLASS_TYPE(WindowResize);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
    
  private:
    uint32_t width_, height_;
  };
  
  /// Window Close event
  class WindowCloseEvent : public Event {
  public:
    /// Default Destructor
    virtual ~WindowCloseEvent() = default;
    
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Window Close Event triggered ");
    }
    
    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
  };
  
} // namespace ikan
