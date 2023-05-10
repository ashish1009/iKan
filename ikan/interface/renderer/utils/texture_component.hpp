//
//  texture_component.h
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  /// This structure holds the texture component with use flag
  struct TextureComponent {
    bool use = false;
    float tiling_factor = 1.0f;
    std::vector<std::shared_ptr<Texture>> texture;
    
    // Animation Sprite Data
    int32_t speed = 15;
    int32_t anim_idx = 0; // No need to copy or save in scene. always starts from 0

    void ClearTextures() { texture.clear(); }

    TextureComponent(const std::shared_ptr<Texture>& tex = nullptr, bool use = true);
    DEFINE_COPY_MOVE_CONSTRUCTORS(TextureComponent)
    
    template<typename UIFunction>
    /// This function renders the texture components
    /// - Parameters:
    ///   - color: color of the texture
    ///   - ui_function: function to render below texture Use
    void RenderGui(glm::vec4& color, UIFunction ui_function) {
      // Quad Color
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
      ui_function();
      
      ImGui::NextColumn();
      ImGui::Text("Quad Color");
      
      ImGui::Columns(1);
      ImGui::Separator();
      
      // Render All texture Component Data
      RenderGuiWork();
    }
    
  protected:
    /// This function renders the GUI data Other then Function UI
    void RenderGuiWork();
    /// This function renders the GUI for loading Texture (Single Texture)
    /// - Parameters:
    ///   - texture: texture referrence
    ///   - is_animation: is texture anumated (already have multiple texture)
    ///   - deleted_texture: deleted texture reference
    bool LoadTextureIcon(std::shared_ptr<Texture>& texture, bool* is_animation, std::shared_ptr<Texture>& deleted_texture);
    /// This wraps the Load texture vector in component
    /// - Parameter texture_vector: tecture vector
    bool LoadTextureIconWrapper(std::vector<std::shared_ptr<Texture>>& texture_vector);
  };
  
  /// This structure holds the property of sprite component (Sub Texture)
  struct SpriteComponent : public TextureComponent {
    enum class Type { Sprite = 0, Animation = 1 };

    bool linear_edge = true;
    bool use_sub_texture = false;
    
    std::vector<std::shared_ptr<SubTexture>> sprite_images;
    
    void ClearSprites() { sprite_images.clear(); }
    void ResetAnimIndx() {
      if (use_sub_texture) {
        if (anim_idx >= speed * sprite_images.size() or anim_idx < 1)
          anim_idx = 0;
      }
      else {
        if (anim_idx >= speed * texture.size() or anim_idx < 1)
          anim_idx = 0;
      }
    }
    
    SpriteComponent(const std::shared_ptr<Texture>& comp = nullptr, bool use = true);
    ~SpriteComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(SpriteComponent)
    
    template<typename UIFunction>
    /// This function renders the texture components
    /// - Parameters:
    ///   - color: color of the texture
    ///   - ui_function: function to render below texture Use
    void RenderGui(glm::vec4& color, UIFunction ui_function) {
      // Quad Color
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
      ui_function();
      
      ImGui::NextColumn();
      ImGui::Text("Quad Color");
      
      ImGui::Columns(1);
      ImGui::Separator();
      
      // Render All texture Component Data
      RenderGuiWork();
    }
    
  private:
    /// This function renders the GUI data Other then Function UI
    void RenderGuiWork();
    /// This function loads the textrue and sprite again
    /// - Parameters:
    ///   - textures: Texture vecote
    ///   - linear_edge: linear edge filter
    void LoadSprite(const std::vector<std::shared_ptr<Texture>>& textures,
                    const std::vector<std::shared_ptr<SubTexture>>& sprites, bool linear_edge);
    /// This function changes the linear flag of texture
    void ChangeLinearTexture();

    /// This function Renders SubTexture Gui
    void SubtextureGui();
    /// This function Renders Sprite Gui
    void SpriteGui();
    /// This function Renders Animatiom Gui
    void SpriteAnimationGui();
  };
  
} // namespace ikan
