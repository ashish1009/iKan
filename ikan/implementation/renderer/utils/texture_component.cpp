//
//  texture_component.cpp
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#include "texture_component.hpp"

namespace ikan {
  
  TextureComponent::TextureComponent(const std::shared_ptr<Texture>& comp, bool use) : use(use), texture(comp) { }
  TextureComponent::TextureComponent(const TextureComponent& other) : use(other.use), tiling_factor(other.tiling_factor) {
    if (other.texture) texture = Renderer::GetTexture(other.texture->GetfilePath());
    IK_CORE_INFO(LogModule::Texture, "Copying TextureComponent");
  }
  
  TextureComponent::TextureComponent(TextureComponent&& other) : use(other.use), tiling_factor(other.tiling_factor) {
    if (other.texture) texture = Renderer::GetTexture(other.texture->GetfilePath());
    IK_CORE_INFO(LogModule::Texture, "Moving TextureComponent");
  }
  
  TextureComponent& TextureComponent::operator=(const TextureComponent& other) {
    if (other.texture) texture = Renderer::GetTexture(other.texture->GetfilePath());
    use = other.use;
    tiling_factor = other.tiling_factor;
    IK_CORE_INFO(LogModule::Texture, "Copying TextureComponent (=operator)");
    return *this;
  }
  
  TextureComponent& TextureComponent::operator=(TextureComponent&& other) {
    if (other.texture) texture = Renderer::GetTexture(other.texture->GetfilePath());
    use = other.use;
    tiling_factor = other.tiling_factor;
    IK_CORE_INFO(LogModule::Texture, "Moving TextureComponent (=operator)");
    return *this;
  }
  
  SpriteComponent::SpriteComponent(const std::shared_ptr<Texture>& tex, bool use_tex) {
    IK_CORE_TRACE(LogModule::Texture, "Creating SpriteComponent");
    use = use_tex;
    texture = tex;
    if (texture) sprite_images.emplace_back(SubTexture::CreateFromCoords(texture, {0.0f, 0.0f}));
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
    LoadTexture(other);
    
    for (const auto& sprite : other.sprite_images) {
      sprite_images.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(), sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Copying SpriteComponent");
  }
  
  SpriteComponent::SpriteComponent(SpriteComponent&& other)
  : linear_edge(other.linear_edge) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    speed = other.speed;
    anim_idx = other.anim_idx;
    LoadTexture(other);
        
    for (const auto& sprite : other.sprite_images) {
      sprite_images.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Moving SpriteComponent");
  }
  
  SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    speed = other.speed;
    linear_edge = other.linear_edge;
    anim_idx = other.anim_idx;
    LoadTexture(other);
    
    for (const auto& sprite : other.sprite_images) {
      sprite_images.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Copying SpriteComponent (=operator)");
    return *this;
  }
  
  SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) {
    use = other.use;
    use_sub_texture = other.use_sub_texture;
    linear_edge = other.linear_edge;
    speed = other.speed;
    anim_idx = other.anim_idx;
    LoadTexture(other);
    
    for (const auto& sprite : other.sprite_images) {
      sprite_images.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Moving SpriteComponent (=operator)");
    return *this;
  }
  
  void SpriteComponent::LoadTexture(const SpriteComponent& other) {
    texture = nullptr;
    ClearSprites();
    
    if (other.texture) {
      texture = Renderer::GetTexture(other.texture->GetfilePath(), other.linear_edge);
      for (const auto& sprite : other.sprite_images) {
        sprite_images.emplace_back(SubTexture::CreateFromCoords(texture, sprite->GetCoords(), sprite->GetSpriteSize(), sprite->GetCellSize()));
      }
    }
  }
  
  void SpriteComponent::ChangeLinearTexture() {
    // Need to copy so not using reference as texture will deleted
    const std::string tex_path = texture->GetfilePath();
    
    texture.reset();
    texture = Renderer::GetTexture(tex_path, linear_edge);
    
    for (auto& sprite : sprite_images) {
      const glm::vec2 coords = sprite->GetCoords();
      const glm::vec2 sprite_size = sprite->GetSpriteSize();
      const glm::vec2 cell_size = sprite->GetCellSize();

      sprite = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
    }
  }
  
} // namespace ikan
