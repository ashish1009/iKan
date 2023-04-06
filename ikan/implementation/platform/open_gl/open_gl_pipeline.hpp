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

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLPipeline);
    
  private:
    RendererID renderer_id_ = 0;
  };
  
} // namespace ikan
