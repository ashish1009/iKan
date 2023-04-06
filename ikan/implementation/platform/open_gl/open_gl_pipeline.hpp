//
//  open_gl_pipeline.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/pipeline.hpp"

namespace ikan {
  
  class OpenGLPipeline : public Pipeline {
  public:
    /// This constructor create the Vertex Array in Graphics to store the buffers
    OpenGLPipeline();
    /// This destructor deletes the Vertex Array and its buffers
    ~OpenGLPipeline();
    
    /// Bind the current pipeline to the GPU
    void Bind() const override;
    /// Unbind the current pipeline to the GPU
    void Unbind() const override;
    /// This function add the Vertex Buffer inside Pipeline and set attribute of each vertices in GPU
    /// - Parameter vertexBuffer: Ref type of Vertex Buffer
    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffers) override;
    /// This function updates the current Index Buffer inside the Pipeline
    /// - Parameter indexBuffer Ref type of Index Buffer
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

    /// This function returns all the Vertex Buffer Stored in Pipeline
    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return vertex_buffers_; }
    /// This function returns the Current Index Buffer Stored in Pipeline
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return index_buffer_; }
    /// This function returns the renderer ID of Vertex Buffer
    RendererID GetRendererID() const override { return renderer_id_; }

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLPipeline);
    
  private:
    RendererID renderer_id_ = 0;
    std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
    std::shared_ptr<IndexBuffer> index_buffer_;
  };
  
} // namespace ikan
