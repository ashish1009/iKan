//
//  graphics_context.hpp
//  ikan
//
//  Created by Ashish . on 03/04/23.
//

#pragma once

#include <GLFW/glfw3.h>

namespace ikan {
  
  /// This class is the Interface for Graphics Context. Instance to be created based on the current API Supported
  class GraphicsContext {
  public:
    /// This function creates the instance of Graphics Context based in the API configured in the Renrerer Class.
    /// - Parameter window: GLFW Window pointer
    static std::unique_ptr<GraphicsContext> Create(GLFWwindow* window);

    /// Default virtual constructor
    virtual ~GraphicsContext() noexcept = default;

    /// This function initialises the Graphics context as per supported API
    virtual void Init() = 0;
    /// This function swaps the graphics buffer. This should be called each frame
    virtual void SwapBuffers() = 0;
  };
  
} // namespace ikan
