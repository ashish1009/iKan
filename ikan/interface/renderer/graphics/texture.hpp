//
//  texture.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"
#include "editor/property_grid.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace ikan {
  
  enum class TextureFormat  {
    None = 0, RGBA
  };

  class Texture {
  public:
    /// This static functions creates Emptry Texture with user Defined Data of size height and Width
    /// - Parameters:
    ///   - width: Width of Empty
    ///   - height Height of Empty Texture
    ///   - data: Data to be stored in Empty Texture
    ///   - size: Size of type of data stored in Texture
    static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
    /// This static functions creates the Texture from image file
    /// - Parameters:
    ///   - file_path: path of texture file
    ///   - linear: min linear flag
    static std::shared_ptr<Texture> Create(const std::string& file_path, bool linear = true);

    virtual ~Texture() = default;
    
    /// This function binds the Current Texture to a slot of shader
    /// - Parameter slot: Slot of shader
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the Current Texture from shader slot
    virtual void Unbind() const = 0;

    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the File Path of Texture NOTE: Return "" for white texture
    virtual const std::string& GetfilePath() const = 0;
    /// This function returns name of texture
    virtual const std::string& GetName() const = 0;
  };
  
  /// Wrepper class to load texture and render as sprite
  class SubTexture {
  public:
    /// This Funtion create the subtexture instance with texture reference pointer and following params
    /// - Parameters:
    ///   - sprite_image: sprite_image
    ///   - min: min bound of sub texture
    ///   - max: mxn bound of sub texture
    ///   - coords: coordinate sof sub image from sprite image with reference to the cell Size.
    ///              e.g {1, 2} -> from top left corner of image use the block of size 'cell size'
    ///             from 16th pixel frol eft and 32nd pixled from top (using 16 assuming cell size is {16, 17})
    ///   - sprite_size: number of blocks to be taken at one time from sprite
    ///   - cell_size: size of block in pixel to be used for 1 block
    SubTexture(const std::shared_ptr<Texture>& sprite_image,
               const glm::vec2& min,
               const glm::vec2& max,
               const glm::vec2& coords = {0.0f, 0.0f},
               const glm::vec2& sprite_size = {1.0f, 1.0f},
               const glm::vec2& cell_size = {16.0f, 16.0f});
    
    /// Default destructor
    ~SubTexture() = default;
    
    /// This function returns the texture reference of sprite
    std::shared_ptr<Texture> GetSpriteImage();
    /// This function returns Texture coordinates of Subtexture
    const glm::vec2* GetTexCoord() const;
    /// This function returns Sprite size of Subtexture
    glm::vec2& GetSpriteSize();
    /// This function returns Cell Size of Subtexture
    glm::vec2& GetCellSize();
    /// This function returnsCoordinates of Subtexture
    glm::vec2& GetCoords();
    
    /// This Funtion create the subtexture instance with texture reference pointer and following params
    /// - Parameters:
    ///   - sprite_image: sprite_image
    ///   - coords: coordinate sof sub image from sprite image with reference to the cell Size.
    ///              e.g {1, 2} -> from top left corner of image use the block of size 'cell size'
    ///             from 16th pixel frol eft and 32nd pixled from top (using 16 assuming cell size is {16, 17})
    ///   - sprite_size: number of blocks to be taken at one time from sprite
    ///   - cell_size: size of block in pixel to be used for 1 block
    static std::shared_ptr<SubTexture> CreateFromCoords(const std::shared_ptr<Texture>& sprite_image,
                                                        const glm::vec2& coords,
                                                        const glm::vec2& sprite_size = {1.0f, 1.0f},
                                                        const glm::vec2& cell_size = {16.0f, 16.0f});
    
  private:
    std::shared_ptr<Texture> sprite_image_;
    glm::vec2 texture_coord_[4];
    glm::vec2 sprite_size_;
    glm::vec2 cell_size_;
    glm::vec2 coords_;
  };

  /// This class is the Interface for Storing Renderer Char Texture data. Implementation is depending on the Supported Renerer API.
  class CharTexture {
  public:
    /// This function creates Emptry Texture with user Defined Data of size height and Width
    /// - Parameters:
    ///   - face: face of char
    ///   - size: size of char
    ///   - bearing: bearing
    ///   - advance: advance
    ///   - char_val: character value
    static std::shared_ptr<CharTexture> Create(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing,
                                               uint32_t advance, [[maybe_unused]] char char_val);

    virtual ~CharTexture() = default;
    
    /// This function binds the Current Texture to a slot of shader
    /// - Parameter slot: Slot of shader
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the Current Texture from shader slot
    virtual void Unbind() const = 0;
    
    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the Size of Freetpe face
    virtual glm::ivec2 GetSize() const = 0;
    /// This function returns the Bearing of Freetpe face
    virtual glm::ivec2 GetBearing() const = 0;
    /// This function returns the Advance of Freetpe face
    virtual uint32_t GetAdvance() const = 0;
  };
  
  /// This class stores the compiled Texture in library
  class TextureLibrary {
  private:
    /// This function returns the Ref type of ikan::Texture. It creates a new if not present in the map
    /// - Parameters:
    ///   - path: path of textre
    ///   - linear: min linear flag
    static std::shared_ptr<Texture> GetTexture(const std::string& path, bool linear = true);
    /// This function deletes all the Texture present int the map
    static void ResetTextures();
    
    // Array of 2 to keep both linear and nearest min and mag flags
    // 0 -> Linear Filter
    // 1 -> Nearest Filter
    static std::unordered_map<std::string, std::array<std::shared_ptr<Texture>, 2>> texture_library_;
    
    MAKE_PURE_STATIC(TextureLibrary)
    
    friend class Renderer;
  };
  
} // namespace ikan
