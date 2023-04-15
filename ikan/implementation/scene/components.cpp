//
//  components.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "components.hpp"
#include "property_grid.hpp"

namespace ikan {
  
#define ComponentLog 1
  
#if ComponentLog
  #define COMP_LOG(...)       IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
  #define COMP_COPY_LOG(...)  IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
#else
  #define COMP_LOG(...)
  #define COMP_COPY_LOG(...)
#endif
  
  // -------------------------------------------------------------------------
  // ID Component
  // -------------------------------------------------------------------------
  IDComponent::IDComponent(const UUID& id) : id(id) { COMP_LOG("Creating ID Component"); }
  IDComponent::~IDComponent() { COMP_LOG("Destroying ID Component"); }
  IDComponent::IDComponent(const IDComponent& other) : id(other.id) { COMP_COPY_LOG("Copying ID Component"); }
  IDComponent::IDComponent(IDComponent&& other) : id(other.id) { COMP_COPY_LOG("Moving ID Component"); }
  IDComponent& IDComponent::operator=(const IDComponent& other) {
    COMP_COPY_LOG("Copying with = operator ID Component");
    id = other.id;
    return *this;
  }
  IDComponent& IDComponent::operator=(IDComponent&& other) {
    COMP_COPY_LOG("Moving with = operator ID Component");
    id = other.id;
    return *this;
  }
  
  // -------------------------------------------------------------------------
  // Tag Component
  // -------------------------------------------------------------------------
  TagComponent::TagComponent(const std::string& tag) : tag(tag) { COMP_LOG("Creating Tag Component"); }
  TagComponent::~TagComponent() { COMP_LOG("Destroying Tag Component"); }
  TagComponent::TagComponent(const TagComponent& other) : tag(other.tag) { COMP_COPY_LOG("Copying Tag Component"); }
  TagComponent::TagComponent(TagComponent&& other) : tag(other.tag) { COMP_COPY_LOG("Moving Tag Component"); }
  TagComponent& TagComponent::operator=(const TagComponent& other) {
    COMP_COPY_LOG("Copying with = operator Tag Component");
    tag = other.tag;
    return *this;
  }
  TagComponent& TagComponent::operator=(TagComponent&& other) {
    COMP_COPY_LOG("Moving with = operator Tag Component");
    tag = other.tag;
    return *this;
  }
  
  // -------------------------------------------------------------------------
  // Transform Component
  // -------------------------------------------------------------------------
  TransformComponent::TransformComponent() {
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
    COMP_LOG("Creating Transform Component");
  }
  TransformComponent::~TransformComponent() { COMP_LOG("Destroying Transform Component"); }
  TransformComponent::TransformComponent(const TransformComponent& other)
  : position(other.Position()), scale(other.Scale()), rotation(other.Rotation()) {
    COMP_COPY_LOG("Copying Transform Component");
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
  }
  TransformComponent::TransformComponent(TransformComponent&& other)
  : position(other.Position()), scale(other.Scale()), rotation(other.Rotation()) {
    COMP_COPY_LOG("Moving Transform Component");
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
  }
  TransformComponent& TransformComponent::operator=(const TransformComponent& other) {
    COMP_COPY_LOG("Copying with = operator Transform Component");
    position = other.Position();
    scale = other.Scale();
    rotation = other.Rotation();
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
    return *this;
  }
  TransformComponent& TransformComponent::operator=(TransformComponent&& other) {
    COMP_COPY_LOG("Moving with = operator Transform Component");
    position = other.Position();
    scale = other.Scale();
    rotation = other.Rotation();
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
    return *this;
  }
  
  void TransformComponent::RenderGui() {
    if (PropertyGrid::Float3("Translation", position, nullptr, 0.25f, 0.0f, MIN_FLT, MAX_FLT, 80.0f)) {
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }
    glm::vec3 rotation_in_degree = glm::degrees(rotation);
    if (PropertyGrid::Float3("Rotation", rotation_in_degree, nullptr, 0.25f, 0.0f, MIN_FLT, MAX_FLT, 80.0f)) {
      rotation = glm::radians(rotation_in_degree);
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }
    
    if (PropertyGrid::Float3("Scale", scale, nullptr, 0.25f, 1.0f, MIN_FLT, MAX_FLT, 80.0f)) {
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }
    ImGui::Separator();
  }
  
  // -------------------------------------------------------------------------
  // Camera Component
  // -------------------------------------------------------------------------
  CameraComponent::CameraComponent(SceneCamera::ProjectionType proj_type) {
    COMP_LOG("Creating Camera Component");
    camera = std::make_shared<SceneCamera>(proj_type);
  }
  CameraComponent::CameraComponent(const CameraComponent& other)
  : is_primary(other.is_primary), is_fixed_aspect_ratio(other.is_fixed_aspect_ratio) {
    COMP_COPY_LOG("Copying Camera Component");
    camera = std::make_shared<SceneCamera>();
    *(camera.get()) = *(other.camera.get());
  }
  CameraComponent::CameraComponent(CameraComponent&& other)
  : is_primary(other.is_primary), is_fixed_aspect_ratio(other.is_fixed_aspect_ratio) {
    COMP_COPY_LOG("Moving Camera Component");
    camera = std::make_shared<SceneCamera>();
    *(camera.get()) = *(other.camera.get());
  }
  CameraComponent& CameraComponent::operator=(const CameraComponent& other) {
    COMP_COPY_LOG("Copying Camera Component with = operator");
    is_primary = other.is_primary;
    is_fixed_aspect_ratio = other.is_fixed_aspect_ratio;
    camera = std::make_shared<SceneCamera>();
    *(camera.get()) = *(other.camera.get());
    return *this;
  }
  CameraComponent& CameraComponent::operator=(CameraComponent&& other) {
    COMP_COPY_LOG("Moving Camera Component with = operator");
    is_primary = other.is_primary;
    is_fixed_aspect_ratio = other.is_fixed_aspect_ratio;
    camera = std::make_shared<SceneCamera>();
    *(camera.get()) = *(other.camera.get());
    camera = std::move(other.camera);
    return *this;
  }
  void CameraComponent::RenderGui() {
    auto column_width = ImGui::GetWindowContentRegionMax().x / 2;
    ImGui::Columns(2);
    
    ImGui::SetColumnWidth(0, column_width);
    ImGui::Checkbox("Primary", &is_primary); ImGui::SameLine();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, column_width);
    ImGui::Checkbox("Fixed Aspect Ratio", &is_fixed_aspect_ratio);
    
    ImGui::Columns(1);
    
    camera->RenderGui();
    ImGui::Separator();
  }

  // -------------------------------------------------------------------------
  // Quad Component
  // -------------------------------------------------------------------------
  QuadComponent::QuadComponent() { COMP_LOG("Creating Quad Component"); }
  QuadComponent::~QuadComponent() { COMP_LOG("Destroying Quad Component"); }
  QuadComponent::QuadComponent(const QuadComponent& other) : color(other.color) {
    COMP_COPY_LOG("Copying Quad Component");
    sprite = other.sprite;
  }
  QuadComponent::QuadComponent(QuadComponent&& other) : color(other.color) {
    COMP_COPY_LOG("Moving Quad Component");
    sprite = other.sprite;
  }
  QuadComponent& QuadComponent::operator=(const QuadComponent& other) {
    COMP_COPY_LOG("Copying Quad Component with = operator");
    color = other.color;
    sprite = other.sprite;
    return *this;
  }
  QuadComponent& QuadComponent::operator=(QuadComponent&& other) {
    COMP_COPY_LOG("Moving Quad Component with = operator");
    color = other.color;
    sprite = other.sprite;
    return *this;
  }
  void QuadComponent::RenderGui() {
    sprite.RenderGui(color, [this]() {
      ImGui::ColorEdit4("Color ", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
      PropertyGrid::HoveredMsg("Texture filter color");
    });
  }
  
  // -------------------------------------------------------------------------
  // Circle Component
  // -------------------------------------------------------------------------
  CircleComponent::CircleComponent() { COMP_LOG("Creating Circle Component"); }
  CircleComponent::~CircleComponent() { COMP_LOG("Creating Circle Component"); }
  CircleComponent::CircleComponent(const CircleComponent& other)
  : color(other.color), thickness(other.thickness), fade(other.fade) {
    COMP_COPY_LOG("Copying Circle Component");
    if (other.texture_comp.texture) {
      texture_comp.use = other.texture_comp.use;
      texture_comp.tiling_factor = other.texture_comp.tiling_factor;
      texture_comp.texture = Renderer::GetTexture(other.texture_comp.texture->GetfilePath());
    }
  }
  CircleComponent::CircleComponent(CircleComponent&& other)
  : color(other.color), thickness(other.thickness), fade(other.fade) {
    COMP_COPY_LOG("Moving Circle Component");
    if (other.texture_comp.texture) {
      texture_comp.use = other.texture_comp.use;
      texture_comp.tiling_factor = other.texture_comp.tiling_factor;
      texture_comp.texture = Renderer::GetTexture(other.texture_comp.texture->GetfilePath());
    }
  }
  CircleComponent& CircleComponent::operator=(const CircleComponent& other) {
    COMP_COPY_LOG("Copying Circle Component with = operator");
    color = other.color;
    thickness = other.thickness;
    fade = other.fade;
    if (other.texture_comp.texture) {
      texture_comp.use = other.texture_comp.use;
      texture_comp.tiling_factor = other.texture_comp.tiling_factor;
      texture_comp.texture = Renderer::GetTexture(other.texture_comp.texture->GetfilePath());
    }
    return *this;
  }
  CircleComponent& CircleComponent::operator=(CircleComponent&& other) {
    COMP_COPY_LOG("Moving Circle Component with = operator");
    color = other.color;
    thickness = other.thickness;
    fade = other.fade;
    if (other.texture_comp.texture) {
      texture_comp.use = other.texture_comp.use;
      texture_comp.tiling_factor = other.texture_comp.tiling_factor;
      texture_comp.texture = Renderer::GetTexture(other.texture_comp.texture->GetfilePath());
    }
    return *this;
  }
  
  void CircleComponent::RenderGui() {
    texture_comp.RenderGui(color, [this]() {
      ImGui::ColorEdit4("Color ", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    });
    ImGui::Separator();
    
    PropertyGrid::Float1("Thickness", thickness, nullptr, 0.05f, 1.0f, 0.05, 1.0f, 100.0f);
    PropertyGrid::Float1("Fade", fade, nullptr, 0.1f, 0.0f, 0.0f, 1.0f, 100.0f);
    ImGui::Separator();
  }
  
} // namespace ikan
