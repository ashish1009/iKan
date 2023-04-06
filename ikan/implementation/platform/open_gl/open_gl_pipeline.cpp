//
//  open_gl_pipeline.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_pipeline.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"
#include <glad/glad.h>

namespace ikan {
  
  OpenGLPipeline::OpenGLPipeline() {
    IDManager::GetPipelineId(renderer_id_);
    glBindVertexArray(renderer_id_);
    IK_CORE_DEBUG(LogModule::Pipeline, "Creating Open GL Pipeline with ID {0} ...", renderer_id_);
  }
  
  OpenGLPipeline::~OpenGLPipeline() {
    IK_CORE_DEBUG(LogModule::Pipeline, "Destroying Open GL Pipeline with ID {0} !!!", renderer_id_);
    IDManager::RemovePipelineId(renderer_id_);
  }
  
  void OpenGLPipeline::Bind() const {
    glBindVertexArray(renderer_id_);
  }
  
  void OpenGLPipeline::Unbind() const {
    glBindVertexArray(0);
  }

} // namespace ikan
