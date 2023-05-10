//
//  texture_component.cpp
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#include "texture_component.hpp"

namespace ikan {
  
  TextureComponent::TextureComponent(const std::shared_ptr<Texture>& comp, bool use) : use(use) {
    texture.push_back(comp);
  }
  TextureComponent::TextureComponent(const TextureComponent& other) : use(other.use), tiling_factor(other.tiling_factor) {
    if (other.texture.size() > 0){
      for (auto& t : other.texture)
        texture.push_back(Renderer::GetTexture(t->GetfilePath()));
    }
    IK_CORE_INFO(LogModule::Texture, "Copying TextureComponent");
  }
  
  TextureComponent::TextureComponent(TextureComponent&& other) : use(other.use), tiling_factor(other.tiling_factor) {
    if (other.texture.size() > 0){
      for (auto& t : other.texture)
        texture.push_back(Renderer::GetTexture(t->GetfilePath()));
    }
    IK_CORE_INFO(LogModule::Texture, "Moving TextureComponent");
  }
  
  TextureComponent& TextureComponent::operator=(const TextureComponent& other) {
    if (other.texture.size() > 0){
      for (auto& t : other.texture)
        texture.push_back(Renderer::GetTexture(t->GetfilePath()));
    }

    use = other.use;
    tiling_factor = other.tiling_factor;
    IK_CORE_INFO(LogModule::Texture, "Copying TextureComponent (=operator)");
    return *this;
  }
  
  TextureComponent& TextureComponent::operator=(TextureComponent&& other) {
    if (other.texture.size() > 0){
      for (auto& t : other.texture)
        texture.push_back(Renderer::GetTexture(t->GetfilePath()));
    }

    use = other.use;
    tiling_factor = other.tiling_factor;
    IK_CORE_INFO(LogModule::Texture, "Moving TextureComponent (=operator)");
    return *this;
  }
  
  bool TextureComponent::LoadTextureIcon(std::shared_ptr<Texture>& texture, bool* is_animation, std::shared_ptr<Texture>& deleted_texture) {
    bool texture_changed = false;
    static std::shared_ptr<Texture> no_texture = Renderer::GetTexture(DM::CoreAsset("textures/default/no_texture.png"));
    size_t tex_id = ((texture) ? texture->GetRendererID() : no_texture->GetRendererID());
    
    // Show the image of texture
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    
    auto& colors = ImGui::GetStyle().Colors;
    const auto& button_hovered = colors[ImGuiCol_ButtonHovered];
    
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 1.0f));
    const auto& button_cctive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(button_cctive.x, button_cctive.y, button_cctive.z, 1.0f));
    
    ImGui::ImageButton((void*)tex_id, ImVec2(40.0f, 40.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    
    if (is_animation) {
      // Right click of mouse option
      if (ImGui::BeginPopupContextItem()) {
        // Delete Coord
        if (ImGui::MenuItem("Delete Texture")) {
          deleted_texture = texture;
        }
        ImGui::EndMenu();
      }
    }
    ImGui::PopStyleColor(3);
    
    // Drop the texture here and load new texture
    PropertyGrid::DropConent([&](const std::string& path)
                             {
      texture.reset();
      texture = Renderer::GetTexture(path);
      texture_changed = true;
    });
    std::string hovered_messge = "Drop the Texture file in the Image Button to add the texture in Animation. \n"
    "Note: Add the Textures in animation order \n"
    "To delete the texture Right click and delete \n";
    
    if (texture)
      hovered_messge += texture->GetfilePath().c_str();
    
    PropertyGrid::HoveredMsg(hovered_messge.c_str());
    return texture_changed;
  }
  
  bool TextureComponent::LoadTextureIconWrapper(std::vector<std::shared_ptr<Texture>>& texture_vector) {
    bool texture_changed = false;
    bool delete_texture = false;
    std::shared_ptr<Texture> deleted_texture;
    
    float main_width = ImGui::GetWindowContentRegionWidth();
    ImVec2 init_pos = ImGui::GetCursorPos();
    
    int32_t item_idx = 0;
    int32_t line = 0;
    for(auto& texture : texture_vector) {
      // Update the cursor for each folder/file based on its position
      float cursor_x = ImGui::GetCursorPos().x + ((50.0f) * item_idx);
      float cursor_y = init_pos.y + line * 100;
      if (cursor_x + 50.0f > main_width) {
        line++;
        item_idx = 0;
        cursor_x = ImGui::GetCursorPos().x + ((50.0f) * item_idx);;
        cursor_y = init_pos.y + line * 100;
      }
      ImGui::SetCursorPos(ImVec2(cursor_x, cursor_y));
      
      texture_changed = LoadTextureIcon(texture, &delete_texture, deleted_texture);
      item_idx++;
    }
    ImGui::SameLine();
    
    bool new_texture_loaded = false;
    std::shared_ptr<Texture> t;
    texture_changed = LoadTextureIcon(t, nullptr, deleted_texture);
    new_texture_loaded = texture_changed;
    
    if (new_texture_loaded) {
      texture_vector.push_back(t);
    }
    
    if (deleted_texture) {
      auto it = std::find(texture_vector.begin(), texture_vector.end(), deleted_texture);
      if (it != texture_vector.end()) {
        texture_vector.erase(it);
      }
    }
    
    return texture_changed;
  }
  
  void TextureComponent::RenderGuiWork() {
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
  
  SpriteComponent::SpriteComponent(const std::shared_ptr<Texture>& tex, bool use_tex) {
    IK_CORE_TRACE(LogModule::Texture, "Creating SpriteComponent");
    use = use_tex;
    ClearTextures();
    
    if (tex) {
      texture.push_back(tex);
    }
    
    if (texture.size() > 0)
      sprite_images.emplace_back(SubTexture::CreateFromCoords(texture.at(0), {0.0f, 0.0f}));
  }
  
  SpriteComponent::~SpriteComponent() {
    IK_CORE_TRACE(LogModule::Texture, "Destroying SpriteComponent");
    ClearSprites();
  }
  
  SpriteComponent::SpriteComponent(const SpriteComponent& other)
  : linear_edge(other.linear_edge) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    speed = other.speed;
    anim_idx = other.anim_idx;
    LoadSprite(other.texture, other.sprite_images, linear_edge);
    
    IK_CORE_TRACE(LogModule::Texture, "Copying SpriteComponent");
  }
  
  SpriteComponent::SpriteComponent(SpriteComponent&& other)
  : linear_edge(other.linear_edge) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    speed = other.speed;
    anim_idx = other.anim_idx;
    LoadSprite(other.texture, other.sprite_images, linear_edge);
        
    IK_CORE_TRACE(LogModule::Texture, "Moving SpriteComponent");
  }
  
  SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    speed = other.speed;
    linear_edge = other.linear_edge;
    anim_idx = other.anim_idx;
    LoadSprite(other.texture, other.sprite_images, linear_edge);
    
    IK_CORE_TRACE(LogModule::Texture, "Copying SpriteComponent (=operator)");
    return *this;
  }
  
  SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    linear_edge = other.linear_edge;
    speed = other.speed;
    anim_idx = other.anim_idx;
    LoadSprite(other.texture, other.sprite_images, linear_edge);
    
    IK_CORE_TRACE(LogModule::Texture, "Moving SpriteComponent (=operator)");
    return *this;
  }
  
  void SpriteComponent::LoadSprite(const std::vector<std::shared_ptr<Texture>>& textures,
                                   const std::vector<std::shared_ptr<SubTexture>>& sprites, bool linear_edge) {
    ClearTextures();
    ClearSprites();
    
    if (textures.size() > 0) {
      for(auto& t : textures) {
        texture.push_back(Renderer::GetTexture(t->GetfilePath(), linear_edge));
      }
      for (const auto& sprite : sprites) {
        sprite_images.emplace_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                                sprite->GetSpriteSize(), sprite->GetCellSize()));
      }
    }
  }
  
  void SpriteComponent::ChangeLinearTexture() {
    for (auto& t : texture) {
      // Need to copy so not using reference as texture will deleted
      const std::string tex_path = t->GetfilePath();
      
      t.reset();
      t = Renderer::GetTexture(tex_path, linear_edge);
    }
    if (texture.size() > 0) {
      for (auto& sprite : sprite_images) {
        const glm::vec2 coords = sprite->GetCoords();
        const glm::vec2 sprite_size = sprite->GetSpriteSize();
        const glm::vec2 cell_size = sprite->GetCellSize();
        
        sprite = SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size);
      }
    }
  }
  
  void SpriteComponent::SubtextureGui() {
    if (sprite_images.size() == 0)
      return;
    
    static Type type = Type::Sprite;
    ImGui::PushID("Animation/Sprite");
    
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
    
    ImGui::RadioButton("Sprite", ((int32_t*)(&type)), (int32_t)Type::Sprite);
    ImGui::NextColumn();
    
    ImGui::RadioButton("Animation", ((int32_t*)(&type)), (int32_t)Type::Animation);
    
    ImGui::Columns(1);
    ImGui::PopID();
    
    ImGui::Separator();
    
    if (type == Type::Sprite) {
      SpriteGui();
    }
    else if (type == Type::Animation) {
      SpriteAnimationGui();
    }
  }
  
  void SpriteComponent::SpriteGui() {
    auto& sub_texture = sprite_images.at(0);
    glm::vec2 coords = sub_texture->GetCoords();
    glm::vec2 sprite_size = sub_texture->GetSpriteSize();
    glm::vec2 cell_size   = sub_texture->GetCellSize();
    
    if (PropertyGrid::Float2("Coords", coords, nullptr, 0.1f, 0.0f, 0.0f, MAX_FLT, 100)) {
      sub_texture->GetSpriteImage().reset();
      sub_texture = SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size);
    }
    if (PropertyGrid::Float2("Sprite Size", sprite_size, nullptr, 1.0f, 1.0f, 0.0f, MAX_FLT, 100.0f)) {
      sub_texture->GetSpriteImage().reset();
      sub_texture = SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size);
    }
    if (PropertyGrid::Float2("Cell Size", cell_size, nullptr, 8.0f, 16.0f, 0.0f, MAX_FLT, 100.0f)) {
      sub_texture->GetSpriteImage().reset();
      sub_texture = SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size);
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
          sub_texture = SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size);
        }
        
        ImGui::EndTooltip();
      } // If Item Hovered
      ImGui::TreePop();
    } // if Sprite Image Open
  }
  
  void SpriteComponent::SpriteAnimationGui() {
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
    static glm::vec2 coords = {0, 0}, sprite_size = {1, 1}, cell_size = {16, 16};
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
        sprite_images.push_back(SubTexture::CreateFromCoords(texture.at(0), coords, sprite_size, cell_size));
    }
    
    static bool delete_sprite = false;
    auto delete_it = sprite_images.begin();
    if (open) {      
      PropertyGrid::Float2("Add Coord", coords, nullptr, 0.1f, 0.0f, 0.0f, MAX_FLT, 100.0f);
      PropertyGrid::Float2("Sprite Size", sprite_size, nullptr, 1.0f, 1.0f, 0.0f, MAX_FLT, 100.0f);
      PropertyGrid::Float2("Cell Size", cell_size, nullptr, 8.0f, 16.0f, 0.0f, MAX_FLT, 100.0f);
      
      ImGui::Separator();
      
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

  void SpriteComponent::RenderGuiWork() {
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
      
      // Texture ANimation Data
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
    
    // Render the Texture Vector Icon to load Texture from File
    if (LoadTextureIconWrapper(texture)) {
      use_sub_texture = false;
      ClearSprites();
      sprite_images.emplace_back(SubTexture::CreateFromCoords(texture.at(0), {0, 0}));
    }
    
    // Render Sub texture Data
    if (use and use_sub_texture and texture.size() > 0) {
      // Selection of type Animation or Sprite
      ImGui::Separator();
      SubtextureGui();
    }
    ImGui::PopID();
  }
  
} // namespace ikan
