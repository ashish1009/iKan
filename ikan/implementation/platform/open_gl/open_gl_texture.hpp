//
//  open_gl_texture.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  /// Implementation for Open GL Texture class
  class OpenGLTexture : public Texture {
  public:
    /// This Constructor create the empty Open GL texture with user data
    /// - Parameters:
    ///   - width: widht of texture
    ///   - height: height of texture
    ///   - data: user data to fill in texture
    ///   - size: size of texture
    OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size);
    /// This constructor creates the texture using image file path
    /// - Parameters:
    ///   - file_path: Texture image file path (absolute)
    ///   - linear: min linear filter
    OpenGLTexture(const std::string& file_path, bool linear);

    /// Default destructor that delete the texture
    virtual ~OpenGLTexture();
    
  private:
    RendererID renderer_id_ = 0;
    
    bool uploaded_ = false;
    
    int32_t width_ = 0, height_ = 0;
    int32_t channel_ = 0;
    uint32_t size_ = 0;
    uint32_t internal_format_ = 0, data_format_ = 0;
    
    void* texture_data_;
    std::string file_path_ = "", name_ = "";
  };
  
} // namespace ikan
