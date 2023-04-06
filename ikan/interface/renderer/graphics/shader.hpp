//
//  shader.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  /// Maximum Texture slot supported by Senderer Shader Current Open GL Version 4.1 supports only 16 Texture slot in Shader
  /// TODO: Change this value based on the Open GL Version
  static constexpr uint32_t kMaxTextureSlotsInShader = 16;
  
  /// This is interface class to complile graphics shader
  class Shader {
  public:
    /// This static function creates Shader Instance based on the Suported API
    /// - Parameter path: Absolute Path of shader
    static std::shared_ptr<Shader> Create(const std::string& file_path);

    virtual ~Shader() noexcept = default;
  };
  
  /// This class stores the compiled shader in library
  class ShaderLibrary {
  private:
    /// This function returns the Ref type of ikan::Shader. It creates a new if not present in the map
    /// - Parameter path: path of shader
    static std::shared_ptr<Shader> GetShader(const std::string& path);
    /// This function deletes all the shaders present int the map
    static void ResetShaders();
    
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shader_library_;
        
    friend class Renderer;
    MAKE_PURE_STATIC(ShaderLibrary)
  };
  
} // namespace ikan
