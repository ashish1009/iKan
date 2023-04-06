//
//  open_gl_texture.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_texture.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"
#include "renderer/utils/renderer_stats.hpp"

#include <stb_image.h>
#include <glad/glad.h>

namespace ikan {
  
  namespace texture_utils {
    
#ifdef IK_DEBUG_FEATURE
    
    std::string GetFormatNameFromEnum(uint32_t format) {
      if (format == GL_RGBA8)       return "GL_RGBA8";
      if (format == GL_RGBA)        return "GL_RGBA";
      if (format == GL_RGB8)        return "GL_RGB8";
      if (format == GL_RGB)         return "GL_RGB";
      if (format == GL_RED)         return "GL_RED";
      if (format == GL_R32I)        return "GL_R32I";
      if (format == GL_RED_INTEGER) return "GL_RED_INTEGER";
      
      if (format == GL_DEPTH_COMPONENT)   return "GL_DEPTH_COMPONENT";
      else IK_CORE_ASSERT(false, "Add New Featured format herer too");
    }
    
#endif
    
    GLint ikanFormatToOpenGLFormat(TextureFormat format) {
      switch (format) {
        case TextureFormat::None:    return (GLint)0;
        case TextureFormat::RGBA:    return GL_RGBA;
      }
      return (GLint)0;
    }
    
    GLint GetTextureType(GLint format_type) {
      switch (format_type) {
        case GL_RGBA8:
        case GL_RGB8:
        case GL_RGBA:
        case GL_RED:
        case GL_R32I:
          return GL_UNSIGNED_BYTE;
          
        case GL_DEPTH_COMPONENT: return GL_FLOAT;
          
        default:
          IK_CORE_ASSERT(false, "Add other formats");
      }
    }
    
  } // namespace texture_utils
  
  OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size)
  : width_((int32_t)width), height_((int32_t)height),
  internal_format_(GL_RGBA8), data_format_(GL_RGBA), size_(size) {
    // Create the buffer to store the white texture
    texture_data_ = new uint32_t;
    memcpy(texture_data_, data, size_);
    
    IDManager::GetTextureId(&renderer_id_);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
    
    // Setup Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    uint16_t bpp = data_format_ == GL_RGBA ? 4 : 3;
    IK_CORE_ASSERT((size_ == (uint32_t)width_ * (uint32_t)height_ * bpp),
                   "Data must be entire texture");
    
    // Create texture in the renderer Buffer
    glTexImage2D(GL_TEXTURE_2D,
                 0, // level
                 (GLint)internal_format_,
                 (GLsizei)width_,
                 (GLsizei)height_,
                 0, // Border
                 data_format_,
                 texture_utils::GetTextureType(internal_format_),
                 (stbi_uc*)(texture_data_));
    
    // Set the flag if uploaded
    uploaded_ = true;
    
    // Increment the size in stats
    RendererStatistics::Get().texture_buffer_size += size_;
    
    IK_CORE_DEBUG(LogModule::Texture, "Creating Open GL White Texture ... ");
    IK_CORE_DEBUG(LogModule::Texture, "  Renderer ID        {0}  ", renderer_id_);
    IK_CORE_DEBUG(LogModule::Texture, "  Width              {0}  ", width_);
    IK_CORE_DEBUG(LogModule::Texture, "  Height             {0}  ", height_);
    IK_CORE_DEBUG(LogModule::Texture, "  Size               {0} B", size_);
    IK_CORE_DEBUG(LogModule::Texture, "  Number of Channel  {0}  ", channel_);
    IK_CORE_DEBUG(LogModule::Texture, "  InternalFormat     {0}  ", texture_utils::GetFormatNameFromEnum(internal_format_));
    IK_CORE_DEBUG(LogModule::Texture, "  DataFormat         {0}  ", texture_utils::GetFormatNameFromEnum(data_format_));
    
    delete (uint32_t*)texture_data_;
  }
  
  OpenGLTexture::~OpenGLTexture() {
  }
  
} // namespace ikan
