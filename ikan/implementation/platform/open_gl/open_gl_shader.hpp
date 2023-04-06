//
//  open_gl_shader.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/shader.hpp"

namespace ikan {
  
  /// This is the Implementation of Open GL Shader compiler
  class OpenGLShader : public Shader {
  public:
    static constexpr uint32_t MaxShaderSupported = 3;
    
    /// This constructor creates the Open GL Shader compiler with file path
    /// - Parameter file_path: shader file path
    OpenGLShader(const std::string& file_path);
    /// This destructor destroy the Open GL Shader compiler
    ~OpenGLShader();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLShader);
  };
  
} // namespace ikan
