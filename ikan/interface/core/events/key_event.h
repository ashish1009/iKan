//
//  key_event.h
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

#include "core/events/event.h"
#include "core/key_codes.h"

namespace ikan {
  
  /// Key base event class
  class KeyEvent : public Event {
  public:
    virtual ~KeyEvent() = default;
    
    /// This function returns the key code of event
    Key GetKeyCode() const { return key_code_; }
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);
    
  protected:
    KeyEvent(Key code) : key_code_(code) {}
    Key key_code_;
  };
  
  /// Key Pressed Event class
  class KeyPressedEvent : public KeyEvent {
  public:
    /// Default virtual destructor
    virtual ~KeyPressedEvent() = default;
    
    /// This this Key pressed event constructor, updates the key code and repeated value
    /// - Parameters:
    ///   - key_code: Key code of event:
    ///   - repeated_count: repeat count:
    KeyPressedEvent(Key key_code, int32_t repeated_count)
    : KeyEvent(key_code), repeated_count(repeated_count) {}
    /// This function returns the count of key repeated
    int32_t GetRepeatCount() const { return repeated_count; }
    
    /// This function prints the event detail
    void Log() const override {
      IK_CORE_TRACE(LogModule::Event, "Key Press Event tiggered ");
      IK_CORE_TRACE(LogModule::Event, "  Key code | {0}", (uint32_t)key_code_);
      IK_CORE_TRACE(LogModule::Event, "  Repeated | {0}", repeated_count);
    }
    EVENT_CLASS_TYPE(KeyPressed);
    
  private:
    int32_t repeated_count = 0;
  };
  
  /// Key Released event class
  class KeyReleasedEvent : public KeyEvent {
  public:
    /// Default virtual destructor
    virtual ~KeyReleasedEvent() = default;
    
    /// Key Release Event
    /// - Parameter key_code: Key code that released:
    KeyReleasedEvent(Key key_code) : KeyEvent(key_code) {}
    
    /// This function prints the event detail
    void Log() const override {
      IK_CORE_TRACE(LogModule::Event, "Key Release Event tiggered ");
      IK_CORE_TRACE(LogModule::Event, "  Key code | {0}", (uint32_t)key_code_);
    }
    
    EVENT_CLASS_TYPE(KeyReleased);
  };
  
  /// Key typed Event class
  class KeyTypedEvent : public KeyEvent {
  public:
    /// Default virtual destructor
    virtual ~KeyTypedEvent() = default;
    
    /// Key Type event
    /// - Parameter key_code: Key code pressed:
    KeyTypedEvent(Key key_code) : KeyEvent(key_code) {}
    
    /// This function prints the event detail
    void Log() const override {
      IK_CORE_TRACE(LogModule::Event, "Key Type Event tiggered ");
      IK_CORE_TRACE(LogModule::Event, "  Key code | {0}", (uint32_t)key_code_);
    }
    EVENT_CLASS_TYPE(KeyTyped);
  };
  
} // namespace ikan
