//
//  texture_component.h
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  static inline bool LoadTextureIcon(std::shared_ptr<Texture>& texture) {
    bool texture_changed = false;
    
    static std::shared_ptr<Texture> no_texture = Renderer::GetTexture(DM::CoreAsset("textures/default/no_texture.png"));
    size_t tex_id = ((texture) ? texture->GetRendererID() : no_texture->GetRendererID());
    
    // Show the image of texture
    ImGui::Image((void*)tex_id, ImVec2(40.0f, 40.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f),
                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    
    // Drop the texture here and load new texture
    PropertyGrid::DropConent([&](const std::string& path)
                             {
      texture.reset();
      texture = Renderer::GetTexture(path);
      texture_changed = true;
    });
    PropertyGrid::HoveredMsg("Drop the Texture file in the Image Button to upload the texture");
    return texture_changed;
  }
  
  /// This structure holds the texture component with use flag
  struct TextureComponent {
    bool use = false;
    float tiling_factor = 1.0f;
    std::shared_ptr<Texture> texture = nullptr;
    
    TextureComponent(const std::shared_ptr<Texture>& tex = nullptr, bool use = true);
    DEFINE_COPY_MOVE_CONSTRUCTORS(TextureComponent)
    
    template<typename UIFunction>
    /// This function renders the texture components
    /// - Parameters:
    ///   - color: color of the texture
    ///   - ui_function: function to render below texture Use
    void RenderGui(glm::vec4& color, UIFunction ui_function) {
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 60);
      
      LoadTextureIcon(texture);
      ImGui::NextColumn();
      
      // Check box to togle use of texture
      ImGui::Checkbox("Use ", &use);
      if (use and texture) {
        ImGui::DragFloat("", &tiling_factor, 1.0f, 1.0f, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
      }
      ui_function();
      ImGui::Columns(1);
    }
  };
  
  /// This structure holds the property of sprite component (Sub Texture)
  struct SpriteComponent : public TextureComponent {
    enum class Type { Sprite = 0, Animation = 1 };
    Type type = Type::Sprite;

    bool linear_edge = true;
    
    std::shared_ptr<SubTexture> sub_texture = nullptr;
    
    // Animation Sprite Data
    int32_t speed = 15;
    int32_t anim_idx = 0; // No need to copy or save in scene. always starts from 0
    std::vector<std::shared_ptr<SubTexture>> sprite_images;
    
    void ClearSprites() { sprite_images.clear(); }
    bool IsAnimation() const { return (type == Type::Animation and sprite_images.size() > 0); }
    void ResetAnimIndx() {
      if (anim_idx >= speed * sprite_images.size() or anim_idx < 1)
        anim_idx = 0;
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
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 60);
      
      if (LoadTextureIcon(texture)) {
        sub_texture.reset();
        LoadTexture(texture);
      }
      ImGui::NextColumn();
      
      // Check box to togle use of texture
      ImGui::Checkbox("Use ", &use);
      PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
      if (use and texture) {
        ImGui::SameLine();
        if (ImGui::Checkbox("Linear Edge", &linear_edge)) {
          ChangeLinearTexture();
        }
      }
      ui_function();
      
      if (use) {
        ImGui::SameLine();
        ImGui::DragFloat("", &tiling_factor, 1.0f, 1.0f, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
        ImGui::Columns(1);
        
        // Selection of type Animation or Sprite
        ImGui::Separator();
        
        ImGui::PushID("Animation/Sprite");
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
        
        ImGui::RadioButton("Sprite", ((int32_t*)(&type)), (int32_t)Type::Sprite);
        ImGui::NextColumn();
        
        ImGui::RadioButton("Animation", ((int32_t*)(&type)), (int32_t)Type::Animation);
        
        ImGui::Columns(1);
        ImGui::PopID();
        
        ImGui::Separator();
        
        // Sub Texture Renderer
        if (sub_texture and type == Type::Sprite) {
          glm::vec2 coords = sub_texture->GetCoords();
          glm::vec2 sprite_size = sub_texture->GetSpriteSize();
          glm::vec2 cell_size   = sub_texture->GetCellSize();
          
          if (PropertyGrid::Float2("Coords", coords, nullptr, 0.1f, 0.0f, 0.0f, MAX_FLT, 100)) {
            sub_texture->GetSpriteImage().reset();
            sub_texture = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
          }
          if (PropertyGrid::Float2("Sprite Size", sprite_size, nullptr, 1.0f, 1.0f, 0.0f, MAX_FLT, 100.0f)) {
            sub_texture->GetSpriteImage().reset();
            sub_texture = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
          }
          if (PropertyGrid::Float2("Cell Size", cell_size, nullptr, 8.0f, 16.0f, 0.0f, MAX_FLT, 100.0f)) {
            sub_texture->GetSpriteImage().reset();
            sub_texture = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
          }
          ImGui::Separator();
          
          // Render the title named as entity name
          const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
          bool open = ImGui::TreeNodeEx("Sprite Image", tree_node_flags);
          if (open) {
            size_t tex_id = sub_texture->GetSpriteImage()->GetRendererID();
            
            float tex_width = (float)sub_texture->GetSpriteImage()->GetWidth();
            float tex_height = (float)sub_texture->GetSpriteImage()->GetHeight() ;
            float width = std::min(ImGui::GetContentRegionAvailWidth(), tex_width);
            
            float size_ratio = width / tex_width;
            float height = tex_height * size_ratio;
            
            ImGui::Image((void*)tex_id, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
            
            ImVec2 pos = ImGui::GetCursorScreenPos();
            if (ImGui::IsItemHovered()) {
              ImGui::BeginTooltip();
              
              ImGuiIO& io = ImGui::GetIO();
              
              float region_fixed_x = (float)((int32_t)(sprite_size.x * cell_size.x * size_ratio));
              float region_fixed_y = (float)((int32_t)(sprite_size.y * cell_size.y * size_ratio));
              static float zoom = 10.0f;
              
              float region_x = io.MousePos.x - pos.x - region_fixed_x * 0.5f;
              if (region_x < 0.0f)
                region_x = 0.0f;
              
              else if (region_x > width - region_fixed_x)
                region_x = width - region_fixed_x;
              
              float region_y = pos.y - io.MousePos.y - region_fixed_y * 0.5f;
              if (region_y < 0.0f)
                region_y = 0.0f;
              
              else if (region_y > height - region_fixed_y)
                region_y = height - region_fixed_y;
              
              ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
              ImGui::Text("Max: (%.2f, %.2f)", region_x + region_fixed_x, region_y + region_fixed_y);
              
              ImVec2 uv0 = ImVec2((region_x) / width, (region_y + region_fixed_y) / height);
              ImVec2 uv1 = ImVec2((region_x + region_fixed_x) / width, (region_y) / height);
              
              ImGui::Image((void*)tex_id, ImVec2(region_fixed_x * zoom, region_fixed_y * zoom),
                           uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
              
              if (ImGui::IsMouseClicked(0)) {
                glm::vec3 coords;
                coords.x = (((region_x + region_fixed_x)) / (cell_size.x * size_ratio)) - sprite_size.x;
                coords.y = (((region_y + region_fixed_y)) / (cell_size.y * size_ratio)) - sprite_size.y;
                
                sub_texture->GetSpriteImage().reset();
                sub_texture = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
              }
              
              ImGui::EndTooltip();
            } // If Item Hovered
            ImGui::TreePop();
          } // if Sprite Image Open
        } // if Sub Texture and Use Sub Texture
        
        if (type == Type::Animation) {
          float speed_drag = (float)speed;
          float min_speed = sprite_images.size();
          if (PropertyGrid::Float1("Speed", speed_drag, nullptr, 1.0f, min_speed, min_speed, MAX_FLT, 100.0f))
            speed = (int32_t)speed_drag;
          
          ImGui::Separator();
          
          static glm::vec2 coords = {0, 0}, sprite_size = {1, 1}, cell_size = {16, 16};
          PropertyGrid::Float2("Add Coord", coords, nullptr, 0.1f, 0.0f, 0.0f, MAX_FLT, 100.0f);
          PropertyGrid::Float2("Sprite Size", sprite_size, nullptr, 1.0f, 1.0f, 0.0f, MAX_FLT, 100.0f);
          PropertyGrid::Float2("Cell Size", cell_size, nullptr, 8.0f, 16.0f, 0.0f, MAX_FLT, 100.0f);
          
          ImGui::Separator();

          const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding;
          
          // Render the title named as entity name
          bool open = ImGui::TreeNodeEx("Animation Sprites", tree_node_flags);
          
          // Get the avilable width and height for button position
          ImVec2 content_region_available = ImGui::GetContentRegionAvail();
          float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
          
          // Set the curson position on same line for (X) button
          ImGui::SameLine(content_region_available.x - line_height * 0.5f);
          float content_height = GImGui->Font->FontSize;
          
          const auto& current_cursor_pos = ImGui::GetCursorPos();
          ImGui::SetCursorPos({current_cursor_pos.x, current_cursor_pos.y + content_height / 4});
          
          // Render the button (X) for removing the component
          static std::shared_ptr<Texture> add_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/plus.png"));
          if (PropertyGrid::ImageButton("Add", add_texture->GetRendererID(), { content_height, content_height } )) {
            bool add = true;
            for (const auto& sprite : sprite_images) {
              if (sprite->GetCoords() == coords) {
                add = false;
                break;
              }
            }
            if (add)
              sprite_images.push_back(SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size));
          }
          
          static bool delete_sprite = false;
          auto delete_it = sprite_images.begin();
          if (open) {
            for (auto it = sprite_images.begin(); it != sprite_images.end(); it++) {
              auto& sprite = *it;
              
              std::string sprite_data;
              sprite_data = std::to_string((int32_t)sprite->GetCoords().x) + " , " + std::to_string((int32_t)sprite->GetCoords().y) + " | ";
              sprite_data += std::to_string((int32_t)sprite->GetSpriteSize().x) + " , " + std::to_string((int32_t)sprite->GetSpriteSize().y) + " | ";
              sprite_data += std::to_string((int32_t)sprite->GetCellSize().x) + " , " + std::to_string((int32_t)sprite->GetCellSize().y);
              
              bool coord_open = ImGui::TreeNodeEx(sprite_data.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_OpenOnArrow);
              
              // Right click of mouse option
              if (ImGui::BeginPopupContextItem()) {
                // Delete Coord
                if (ImGui::MenuItem("Delete Coord")) {
                  delete_sprite = true;
                  delete_it = it;
                }
                ImGui::EndMenu();
              }
              
              if (coord_open)
                ImGui::TreePop();
            }
            ImGui::TreePop();
          } // if (open)
          ImGui::Separator();
          
          if (delete_sprite) {
            sprite_images.erase(delete_it);
            delete_sprite = false;
          }
        }
        
      } // If Use Texture
    }
    
  private:
    /// This function loads the textrue and sprite again
    /// - Parameter other: component
    void LoadTexture(const SpriteComponent& other);
    /// This function changes the linear flag of texture
    void ChangeLinearTexture();
  };
  
} // namespace ikan
