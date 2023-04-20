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
    IK_CORE_TRACE(LogModule::Texture, "Creating TextureComponent");
    use = use_tex;
    texture = tex;
    if (texture) sub_texture = SubTexture::CreateFromCoords(texture, {0.0f, 0.0f});
  }
  
  SpriteComponent::~SpriteComponent() {
    IK_CORE_TRACE(LogModule::Texture, "Destroying TextureComponent");
    sub_texture.reset();
    ClearSprites();
  }
  
  SpriteComponent::SpriteComponent(const SpriteComponent& other)
  : linear_edge(other.linear_edge), use_sub_texture(other.use_sub_texture) {
    use = other.use;
    LoadTexture(other);
    
    is_animation = other.is_animation;
    speed = other.speed;
    
    for (const auto& sprite : other.sprites) {
      sprites.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Copying TextureComponent");
  }
  
  SpriteComponent::SpriteComponent(SpriteComponent&& other)
  : linear_edge(other.linear_edge), use_sub_texture(other.use_sub_texture) {
    use = other.use;
    LoadTexture(other);
    
    is_animation = other.is_animation;
    speed = other.speed;
    
    for (const auto& sprite : other.sprites) {
      sprites.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Moving TextureComponent");
  }
  
  SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) {
    use = other.use;
    linear_edge = other.linear_edge;
    use_sub_texture = other.use_sub_texture;
    LoadTexture(other);
    
    is_animation = other.is_animation;
    speed = other.speed;
    
    for (const auto& sprite : other.sprites) {
      sprites.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Copying TextureComponent (=operator)");
    return *this;
  }
  
  SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) {
    use = other.use;
    linear_edge = other.linear_edge;
    use_sub_texture = other.use_sub_texture;
    LoadTexture(other);
    
    is_animation = other.is_animation;
    speed = other.speed;
    
    for (const auto& sprite : other.sprites) {
      sprites.push_back(SubTexture::CreateFromCoords(sprite->GetSpriteImage(), sprite->GetCoords(),
                                                     sprite->GetSpriteSize(), sprite->GetCellSize()));
    }
    IK_CORE_TRACE(LogModule::Texture, "Moving TextureComponent (=operator)");
    return *this;
  }
  
  void SpriteComponent::LoadTexture(const SpriteComponent& other) {
    texture = nullptr;
    sub_texture = nullptr;
    
    if (other.texture) {
      texture = Renderer::GetTexture(other.texture->GetfilePath(), other.linear_edge);
      if (other.sub_texture) {
        sub_texture = SubTexture::CreateFromCoords(texture, other.sub_texture->GetCoords(),
                                                   other.sub_texture->GetSpriteSize(),
                                                   other.sub_texture->GetCellSize());
      }
    }
  }
  
  void SpriteComponent::ChangeLinearTexture() {
    // Need to copy so not using reference as texture will deleted
    const std::string tex_path = texture->GetfilePath();
    const glm::vec2 coords = sub_texture->GetCoords();
    const glm::vec2 sprite_size = sub_texture->GetSpriteSize();
    const glm::vec2 cell_size = sub_texture->GetCellSize();
    
    texture.reset();
    sub_texture.reset();
    
    texture = Renderer::GetTexture(tex_path, linear_edge);
    sub_texture = SubTexture::CreateFromCoords(texture, coords, sprite_size, cell_size);
  }
  
} // namespace ikan
