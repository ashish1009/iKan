//
//  mouse_codes.h
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

namespace ikan {
  
  /// Mouse button codes
  /// - Note: From glfw3.h
  enum class MouseButton : uint8_t {
    Button0                = 0,
    Button1                = 1,
    Button2                = 2,
    Button3                = 3,
    Button4                = 4,
    Button5                = 5,
    Button6                = 6,
    Button7                = 7,
    
    ButtonLast             = Button7,
    ButtonLeft             = Button0,
    ButtonRight            = Button1,
    ButtonMiddle           = Button2
  };
  
} // nemespace ikan
