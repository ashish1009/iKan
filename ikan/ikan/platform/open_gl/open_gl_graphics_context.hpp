//
//  open_gl_graphics_context.hpp
//  ikan
//
//  Created by Ashish . on 03/04/23.
//

#pragma once

#include "renderer/graphics/graphics_context.hpp"

namespace ikan {
  
  /// This is the Implementation class of Open GL Renderer Graphics context
  class OpenGLGraphicsContext : public GraphicsContext {
  public:
    /// This constructor creates the Open GL Context using GLFW 'window' pointer
    /// - Parameter window: GLFW window for which context need to be created
    OpenGLGraphicsContext(GLFWwindow* window);
    /// This destructor destroy the context mapped to window
    virtual ~OpenGLGraphicsContext() noexcept;
    
    /// This function initialize the the Renderer context
    void Init() override;
    /// This function swaps the renderer buffers, Should be called each frame
    void SwapBuffers() override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLGraphicsContext);
    
  private:
    GLFWwindow* window_;
  };
  
} // namespace ikan
