//
//  open_gl_renderer_buffer.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/renderer_buffer.hpp"

namespace ikan {
  
  /// This class is the implementation of Open GL Renderer Vertex Buffer, to store the vertices of the objects.
  class OpenGLVertexBuffer : public VertexBuffer {
  public:
    /// This constructor creates Buffer to store the vertices of the data. This is static data buffer which takes the data pointer in the argument
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    OpenGLVertexBuffer(void* data, uint32_t size);
    /// This constructor creates the Buffer to store the vertices of the data. This is a dynamic data buffer, takes only size of buffer
    /// - Parameter size: size of data
    OpenGLVertexBuffer(uint32_t size);
    /// This destructor destroy the Renderer Index Buffer
    ~OpenGLVertexBuffer();
    
    /// This function update the date in buffer dynamically
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    void SetData(void* data, uint32_t size) override;
    /// This function binds the Vertex Buffer before rendering
    void Bind() const override;
    /// This function unbinds the Vertex Buffer after rendering
    void Unbind() const override;

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLVertexBuffer);

  public:
    RendererID renderer_id_ = 0;
    uint32_t size_ = 0;
  };
  
}
