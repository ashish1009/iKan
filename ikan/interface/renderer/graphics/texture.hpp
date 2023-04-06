//
//  texture.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  enum class TextureFormat  {
    None = 0, RGBA
  };

  class Texture {
  public:
    /// This static functions creates Emptry Texture with user Defined Data of size height and Width
    /// - Parameters:
    ///   - width: Width of Empty
    ///   - height Height of Empty Texture
    ///   - data: Data to be stored in Empty Texture
    ///   - size: Size of type of data stored in Texture
    static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
    /// This static functions creates the Texture from image file
    /// - Parameters:
    ///   - file_path: path of texture file
    ///   - linear: min linear flag
    static std::shared_ptr<Texture> Create(const std::string& file_path, bool linear = true);

    virtual ~Texture() = default;
  };
  
} // namespace ikan
