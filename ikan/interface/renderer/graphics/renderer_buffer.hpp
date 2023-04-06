//
//  renderer_buffer.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  /// This class is the interface of Renderer Vertex Buffer, to store the vertices of the objects.
  class VertexBuffer {
  public:
    /// This static function creates the instance of Vertex Buffer based on the supported API. This is static data buffer which takes the
    /// data pointer in the argument
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    static std::shared_ptr<VertexBuffer> Create(void* data, uint32_t size);
    /// This static function creates the instance of Vertex Buffer based on the supported API. This is a dynamic data buffer, takes only
    /// size of buffer
    /// - Parameter size: size of data
    static std::shared_ptr<VertexBuffer> Create(uint32_t size);

    virtual ~VertexBuffer() = default;
    
    /// This function update the date in buffer dynamically
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    virtual void SetData(void* data, uint32_t size) = 0;
    /// This function binds the Vertex Buffer before rendering
    virtual void Bind() const = 0;
    /// This function unbinds the Vertex Buffer after rendering
    virtual void Unbind() const = 0;

  };
  
} // namespace ikan
