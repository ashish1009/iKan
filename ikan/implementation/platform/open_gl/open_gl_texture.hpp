//
//  open_gl_texture.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"
#include <glad/glad.h>

namespace ikan {
  
  /// Implementation for Open GL Texture class
  class OpenGLTexture : public Texture {
  public:
    /// This Constructor create the empty Open GL texture with user data
    /// - Parameters:
    ///   - width: widht of texture
    ///   - height: height of texture
    ///   - data: user data to fill in texture
    ///   - size: size of texture
    OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size);
    /// This constructor creates the texture using image file path
    /// - Parameters:
    ///   - file_path: Texture image file path (absolute)
    ///   - linear: min linear filter
    OpenGLTexture(const std::string& file_path, bool linear);

    /// Default destructor that delete the texture
    virtual ~OpenGLTexture();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the texture
    void Unbind() const override;

    /// This function returns width of texture
    uint32_t GetWidth() const override { return width_; }
    /// This function returns height of texture
    uint32_t GetHeight() const override { return height_; }
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override { return renderer_id_; }
    /// This function returns file path of texture
    const std::string& GetfilePath() const override { return file_path_; }
    /// This function returns name of texture
    const std::string& GetName() const override { return name_; }

  private:
    RendererID renderer_id_ = 0;
    
    bool uploaded_ = false;
    
    int32_t width_ = 0, height_ = 0;
    int32_t channel_ = 0;
    uint32_t size_ = 0;
    uint32_t internal_format_ = 0, data_format_ = 0;
    
    void* texture_data_;
    std::string file_path_ = "", name_ = "";
  };
  
  /// Implementaiton for Open GL Char Texture
  class OpenGLCharTexture : public CharTexture {
  public:
    /// This constructor creates the Open GL Texture for rendering char
    /// - Parameters:
    ///   - face: face of char
    ///   - size: size of char
    ///   - bearing: bearing
    ///   - advance: advance
    ///   - char_val: character value
    OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance, [[maybe_unused]] char char_val);
    /// This is the default Open GL Char Texture destructor
    virtual ~OpenGLCharTexture();
    
    /// This function binds the texture
    void Bind(uint32_t slot) const override;
    /// This function unbinds the texture
    void Unbind() const  override;
    
    /// This function returns the Renderer ID
    RendererID GetRendererID() const override { return renderer_id_; }
    /// This function returns width of texture
    uint32_t GetWidth() const override { return width_; }
    /// This function returns height of texture
    uint32_t GetHeight() const override { return height_; }
    /// This function returns the size
    glm::ivec2 GetSize() const override { return size_; }
    /// This function returns the bearing
    glm::ivec2 GetBearing() const override { return bearing_; }
    /// This function returns the advance
    uint32_t GetAdvance() const override { return advance_; }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLCharTexture);

  private:
    RendererID renderer_id_;
    glm::ivec2 size_;
    glm::ivec2 bearing_;
    uint32_t advance_ = 0;
    uint32_t data_size_ = 0;
    uint32_t width_ = 0, height_ = 0;
  };
  
  namespace texture_utils {
    
#ifdef IK_DEBUG_FEATURE
    
    /// This function returns the Format name from Enum
    /// - Parameter format: enum taken as uint (enum in Glad)
    std::string GetFormatNameFromEnum(uint32_t format);
    
#endif
    
    /// This function returns the Open GL Format type from i kan type
    /// - Parameter format: i kan type
    GLint ikanFormatToOpenGLFormat(TextureFormat format);
    /// This function returns the texture fype from internal format
    /// - Parameter format_tyoe: format type
    GLint GetTextureType(GLint format_tyoe);
    
  } // namespace texture_utils
  
} // namespace ikan
