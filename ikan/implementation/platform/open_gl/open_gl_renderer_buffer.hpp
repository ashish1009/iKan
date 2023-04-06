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
    /// This function uptate the Buffer layeout value in Vertex Buffer
    /// - Parameter layout: new Buffer layout
    void AddLayout(const BufferLayout& layout) override { layout_ = layout; }

    /// This function returns the renderer ID of Vertex Buffer
    RendererID GetRendererID() const override { return renderer_id_; }
    /// This function returns the Buffer layout stored in Vertex Buffer
    const BufferLayout& GetLayout() const override { return layout_; }
    /// This function returns the size of Vertex Buffer in GPU
    uint32_t GetSize() const override { return size_; }

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLVertexBuffer);

  public:
    RendererID renderer_id_ = 0;
    uint32_t size_ = 0;
    BufferLayout layout_;
  };
  
}
