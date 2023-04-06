//
//  pipeline.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"
#include "renderer/graphics/renderer_buffer.hpp"

namespace ikan {
  
  /// Interface class for Storing Renderer Pipeline. Implementation is depending on the supported Renerer API.
  class Pipeline {
  public:
    /// This static function creates Ref type instance of Pipeline. Instance depending on the current Renderer API
    static std::shared_ptr<Pipeline> Create();

    virtual ~Pipeline() noexcept = default;
    
    /// Bind the current pipeline to the GPU
    virtual void Bind() const = 0;
    /// Unbind the current pipeline to the GPU
    virtual void Unbind() const = 0;

    /// This function add the Vertex Buffer inside Pipeline and set attribute of each vertices in GPU
    /// - Parameter vertexBuffer: Ref type of Vertex Buffer
    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

    /// This function returns all the Vertex Buffer Stored in Pipeline
    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    /// This function returns the renderer ID of Vertex Buffer
    virtual RendererID GetRendererID() const = 0;
  };
  
} // namespace ikan
