//
//  open_gl_graphics_context.cpp
//  ikan
//
//  Created by Ashish . on 03/04/23.
//

#include <glad/glad.h>

#include "open_gl_graphics_context.hpp"
#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
  : window_(window) {
    IK_CORE_ASSERT(window, "Window pointer is NULL !!!");
    IK_CORE_INFO(LogModule::GraphicsContext, "Creating Open GL Renderer Context ... ");
    Init();
  }
  
  OpenGLGraphicsContext::~OpenGLGraphicsContext() {
    IK_CORE_WARN(LogModule::GraphicsContext, "Destroying Open GL Renderer Context !!! ");
  }
  
  void OpenGLGraphicsContext::Init() {
    // Create Renderer Context for GLFW Window
    // NOTE: This function makes the OpenGL or OpenGL ES context of the specified window on the calling calling thread. A context must only be made
    //       current on a single thread at a time and each thread can have only a single context at a time.
    glfwMakeContextCurrent(window_);
    
    // We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. GLFW gives us glfwGetProcAddress that defines
    // the correct function based on which OS we're compiling for
    IK_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Can not initialize the Glad");
  }
  
  void OpenGLGraphicsContext::SwapBuffers() {
    glfwSwapBuffers(window_);
  }
  
} // namespace ikan
