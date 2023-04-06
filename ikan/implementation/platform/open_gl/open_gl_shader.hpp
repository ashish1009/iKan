//
//  open_gl_shader.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/shader.hpp"
#include "platform/open_gl/open_gl_shader_uniforms.hpp"
#include <glad/glad.h>

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
    
  private:
    /// This function reads the shader code in string and store all the shader present in the file in a map to be used later by compiler.
    /// - Parameter source_string: shader code in string
    void PreprocessFile(const std::string& source_string);
    /// This functions compiles all the shader codes and store their ID in Program ID (renderer_id).
    void Compile();

    RendererID renderer_id_ = 0;
    std::string asset_path_ = "", name_ = "";
    std::unordered_map<GLenum, std::string> shader_source_code_map_;
  };
  
} // namespace ikan
