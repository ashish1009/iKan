//
//  mouse_event.h
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/events/event.h"
#include "core/mouse_codes.h"

namespace ikan {
  
  /// Mouse base Event class
  class MouseMovedEvent : public Event {
  public:
    /// Default virtual destructor
    virtual ~MouseMovedEvent() = default;
    /// Mouse moved event
    /// - Parameters:
    ///  - x_offset: x position of Mouse:
    ///  - y_offset: y position of Mouse:
    MouseMovedEvent(float x, float y) : mouse_x_(x), mouse_y_(y) {}
    /// This function returns the X position of mouse
    float GetX() const { return mouse_x_; }
    /// This function returns the Y position of mouse
    float GetY() const { return mouse_y_; }
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Mouse Move Event tiggered ");
      IK_TRACE(LogModule::Event, "  X | {0}", mouse_x_);
      IK_TRACE(LogModule::Event, "  Y | {0}", mouse_y_);
    }
    EVENT_CLASS_TYPE(MouseMoved);
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
  private:
    float mouse_x_, mouse_y_;
  };
  
  /// Mouse scrol event
  class MouseScrolledEvent : public Event {
  public:
    /// Default virtual destructor
    virtual ~MouseScrolledEvent() = default;
    
    /// Mosue Scroll Event
    /// - Parameters:
    ///  - x_offset: x Offset of Scrolling:
    ///  - y_offset: y Offset of Scrolling:
    MouseScrolledEvent(float x_offset, float y_offset) : x_offset_(x_offset), y_offset_(y_offset) {}
    /// This function returns x Offset of Scrolling
    float GetXOffset() const { return x_offset_; }
    /// This function returns y Offset of Scrolling
    float GetYOffset() const { return y_offset_; }
    
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Mouse Scroll Event tiggered ");
      IK_TRACE(LogModule::Event, "  X Offset | {0}", x_offset_);
      IK_TRACE(LogModule::Event, "  Y Offset | {0}", y_offset_);
    }
    EVENT_CLASS_TYPE(MouseScrolled);
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
  private:
    float x_offset_, y_offset_;
  };
  
  /// Mouse button base calss
  class MouseButtonEvent : public Event {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonEvent() = default;
    /// This function returns the Mouse button pressed
    MouseButton GetMouseButton() const { return button_; }
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    
  protected:
    MouseButtonEvent(MouseButton button) : button_(button) {}
    MouseButton button_;
  };
  
  /// Mouse button pressed
  class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonPressedEvent() = default;
    /// Mouse button pressed Event
    /// - Parameter button: Mouse button pressed:
    MouseButtonPressedEvent(MouseButton button) : MouseButtonEvent(button) {}
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Mosue Buttom Press Event tiggered ");
      IK_TRACE(LogModule::Event, "  Button code | {0}", (uint32_t)button_);
    }
    EVENT_CLASS_TYPE(MouseButtonPressed);
  };
  
  /// Mouse button released
  class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonReleasedEvent() = default;
    /// Mouse button release ebent
    /// - Parameter button: Mouse button released:
    MouseButtonReleasedEvent(MouseButton button) : MouseButtonEvent(button) {}
    /// This function prints the event detail
    void Log() const override {
      IK_TRACE(LogModule::Event, "Mouse Button Release Event tiggered ");
      IK_TRACE(LogModule::Event, "  Button code | {0}", (uint32_t)button_);
    }
    EVENT_CLASS_TYPE(MouseButtonReleased);
  };
  
} // namespace ikan
