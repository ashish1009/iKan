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

      // Check box to togle use of texture
      PropertyGrid::CheckBox("Use Texure", use);
      PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
      
      if (use and texture.size() > 0) {
        PropertyGrid::Float1("Tiling Factor", tiling_factor, nullptr, 1.0f, 1.0f, MIN_FLT, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
      }
      ImGui::Separator();
      
      LoadTextureIconWrapper(texture);
    }
    
  protected:
    bool LoadTextureIcon(std::shared_ptr<Texture>& texture, bool* is_animation, std::shared_ptr<Texture>& deleted_texture);
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
      ImGui::PushID("Texture Data");

      // Quad Color
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
      ui_function();
      
      ImGui::NextColumn();
      ImGui::Text("Quad Color");
      
      ImGui::Columns(1);
      ImGui::Separator();
      
      // Check box to togle use of texture
      PropertyGrid::CheckBox("Use Texure", use);
      PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
      
      if (use and texture.size() > 0) {
        if (PropertyGrid::CheckBox("Linear Edge", linear_edge)) {
          ChangeLinearTexture();
        }
        PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
        
        PropertyGrid::CheckBox("Sprite", use_sub_texture);
        PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
        
        PropertyGrid::Float1("Tiling Factor", tiling_factor, nullptr, 1.0f, 1.0f, MIN_FLT, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
        
        bool show_animation_speed = false;
        if (use_sub_texture) {
          if (sprite_images.size() > 1)
            show_animation_speed = true;
        }
        else {
          if (texture.size() > 1)
            show_animation_speed = true;
        }
        if (show_animation_speed) {
          ImGui::Separator();
          float speed_drag = (float)speed;
          float min_speed = (use_sub_texture) ? sprite_images.size() : texture.size();
          if (PropertyGrid::Float1("Animation Speed", speed_drag, nullptr, 1.0f, min_speed, min_speed, MAX_FLT, ImGui::GetWindowContentRegionMax().x / 2))
            speed = (int32_t)speed_drag;
        }
      }
      ImGui::Separator();

      if (LoadTextureIconWrapper(texture)) {
        use_sub_texture = false;
        ClearSprites();
        sprite_images.emplace_back(SubTexture::CreateFromCoords(texture.at(0), {0, 0}));
      }
      
      if (use and use_sub_texture and texture.size() > 0) {
        // Selection of type Animation or Sprite
        ImGui::Separator();
        SubtextureGui();
      }
      ImGui::PopID();
    }
    
  private:
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
    void AnimationGui();
  };
  
} // namespace ikan
