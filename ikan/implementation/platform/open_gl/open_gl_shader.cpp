//
//  open_gl_shader.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_shader.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"

namespace ikan {
  
  OpenGLShader::OpenGLShader(const std::string& file_path)
  : asset_path_(file_path), name_(StringUtils::GetNameFromFilePath(file_path)) {
    IDManager::GetShaderId(renderer_id_);

    IK_CORE_DEBUG(LogModule::Shader, "Creating Open GL Shader ...");
    IK_CORE_DEBUG(LogModule::Shader, "  Renderer ID  {0} ", renderer_id_);
    IK_CORE_DEBUG(LogModule::Shader, "  Name         {0} ", name_);
    IK_CORE_DEBUG(LogModule::Shader, "  File Path    {0} ", asset_path_);
  }
  
  OpenGLShader::~OpenGLShader() {
    
  }

  
} // namespace ikan
