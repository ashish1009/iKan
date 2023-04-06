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
    
    virtual ~Texture() = default;
  };
  
} // namespace ikan
