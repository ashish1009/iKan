//
//  components.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "components.hpp"
#include "scene/core_entity.hpp"
#include "editor/property_grid.hpp"

namespace ikan {
  
#define ComponentLog 1
  
#if ComponentLog
  #define COMP_LOG(...)       IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
  #define COMP_COPY_LOG(...)  IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
#else
  #define COMP_LOG(...)
  #define COMP_COPY_LOG(...)
#endif
  
#define COMP_COPY_MOVE_CONSTRUCTORS(x) \
x::x(const x& other) { \
  Copy(other); \
  COMP_COPY_LOG("Copying {0}", #x); \
} \
x::x(x&& other) { \
  Copy(other); \
  COMP_COPY_LOG("Moving {0}", #x); \
} \
x& x::operator=(const x& other) { \
  COMP_COPY_LOG("Copying with = operator {0}", #x); \
  Copy(other); \
  return *this; \
} \
x& x::operator=(x&& other) { \
  COMP_COPY_LOG("Moving with = operator {0}", #x); \
  Copy(other); \
  return *this; \
} \
  
  // -------------------------------------------------------------------------
  // ID Component
  // -------------------------------------------------------------------------
  IDComponent::IDComponent(const UUID& id) : id(id) { COMP_LOG("Creating ID Component"); }
  IDComponent::~IDComponent() { COMP_LOG("Destroying ID Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(IDComponent);
  void IDComponent::Copy(const IDComponent &other) {
    id = other.id;
  }

  // -------------------------------------------------------------------------
  // Tag Component
  // -------------------------------------------------------------------------
  TagComponent::TagComponent(const std::string& tag) : tag(tag) { COMP_LOG("Creating Tag Component"); }
  TagComponent::~TagComponent() { COMP_LOG("Destroying Tag Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(TagComponent);
  void TagComponent::Copy(const TagComponent &other) {
    tag = other.tag;
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
  COMP_COPY_MOVE_CONSTRUCTORS(TransformComponent);
  
  void TransformComponent::Copy(const TransformComponent &other) {
    position = other.Position();
    scale = other.Scale();
    rotation = other.Rotation();
    transform = Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
  }
  
  void TransformComponent::RenderGui() {
    if (PropertyGrid::Float3("Position", position, nullptr, 0.25f, 0.0f, MIN_FLT, MAX_FLT, 80.0f)) {
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
  CameraComponent::~CameraComponent() { COMP_LOG("Destroying Camera Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(CameraComponent);
  
  void CameraComponent::Copy(const CameraComponent &other) {
    is_primary = other.is_primary;
    is_fixed_aspect_ratio = other.is_fixed_aspect_ratio;
    camera = std::make_shared<SceneCamera>();
    *(camera.get()) = *(other.camera.get());
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
  COMP_COPY_MOVE_CONSTRUCTORS(QuadComponent);
  
  void QuadComponent::Copy(const QuadComponent &other) {
    color = other.color;
    sprite = other.sprite;
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
  COMP_COPY_MOVE_CONSTRUCTORS(CircleComponent);
  
  void CircleComponent::Copy(const CircleComponent &other) {
    color = other.color;
    thickness = other.thickness;
    fade = other.fade;
    if (other.texture_comp.texture) {
      texture_comp.use = other.texture_comp.use;
      texture_comp.tiling_factor = other.texture_comp.tiling_factor;
      texture_comp.texture = Renderer::GetTexture(other.texture_comp.texture->GetfilePath());
    }
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
  
  // -------------------------------------------------------------------------
  // Rigid Body Component
  // -------------------------------------------------------------------------
  RigidBodyComponent::RigidBodyComponent() { COMP_LOG("Creating Rigid Body Component"); }
  RigidBodyComponent::~RigidBodyComponent() { COMP_LOG("Destroying Rigid Body Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(RigidBodyComponent);

  void RigidBodyComponent::Copy(const RigidBodyComponent &other) {
    type = other.type;
    is_ground = other.is_ground;
    is_sensor = other.is_sensor;
    fixed_rotation =  other.fixed_rotation;
    velocity =  other.velocity;
    angular_velocity =  other.angular_velocity;
    linear_damping =  other.linear_damping;
    angular_damping =  other.angular_damping;
    gravity_scale =  other.gravity_scale;
    runtime_body = other.runtime_body;
  }
  
  b2BodyType RigidBodyComponent::B2BodyType(RigidBodyComponent::RbBodyType type) {
    switch (type) {
      case RbBodyType::Static: return b2BodyType::b2_staticBody;
      case RbBodyType::Kinametic: return b2BodyType::b2_kinematicBody;
      case RbBodyType::Dynamic: return b2BodyType::b2_dynamicBody;
      default: IK_ASSERT(false);
    }
  }
  
  void RigidBodyComponent::SetGravityScale(float scale) {
    gravity_scale = scale;
    if (runtime_body != nullptr) {
      ((b2Body*)runtime_body)->SetGravityScale(gravity_scale);
    }
  }
  
  void RigidBodyComponent::SetVelocity(const glm::vec2& vel) {
    if (runtime_body == nullptr)  return;
    velocity = vel;
    ((b2Body*)runtime_body)->SetLinearVelocity({velocity.x, velocity.y});
  }
  
  void RigidBodyComponent::SetAngularVelocity(float ang_vel) {
    if (runtime_body == nullptr)  return;
    angular_velocity = ang_vel;
    ((b2Body*)runtime_body)->SetAngularVelocity(angular_velocity);
  }

  void RigidBodyComponent::ApplyImpulseToCenter(const glm::vec2 &imp) {
    if (runtime_body == nullptr)  return;
    ((b2Body*)runtime_body)->ApplyLinearImpulseToCenter({imp.x, imp.y}, true);
  }
  
  void RigidBodyComponent::RenderGui() {
    RbBodyType new_body_type = RbBodyType(PropertyGrid::ComboDrop("Rigid Body Type", { "Static" , "Kinamatic", "Dynamic" }, (uint32_t)type));
    
    // Render the property based on the projection type of camera
    if (new_body_type != type)
      type = new_body_type;
    
    if (type != RbBodyType::Static) {
      PropertyGrid::Float2("Linear Velocity", velocity, nullptr, 0.1, 0.0f, 0.0f, MAX_FLT);
      PropertyGrid::Float1("Angular Velocity", angular_velocity, nullptr, 0.1, 0.0f, 0.0f, MAX_FLT);
      
      ImGui::Separator();
      PropertyGrid::Float1("Linear Damping", linear_damping, nullptr, 0.1, 0.0f, 0.0f, MAX_FLT);
      PropertyGrid::Float1("Angular Damping", angular_damping, nullptr, 0.1, 0.0f, 0.0f, MAX_FLT);
      
      ImGui::Separator();
      PropertyGrid::Float1("Gravity Scale", gravity_scale, nullptr, 0.1, 0.0f, 0.0f, MAX_FLT);
    }
    
    ImGui::Separator();
    PropertyGrid::CheckBox("Is Ground", is_ground);
    PropertyGrid::CheckBox("Is Sensor", is_sensor);

    if (type != RbBodyType::Static) {
      PropertyGrid::CheckBox("Fixed Rotation", fixed_rotation);
    }
    
    ImGui::Separator();
  }
  
  // -------------------------------------------------------------------------
  // Physics Material
  // -------------------------------------------------------------------------
  PhysicsMaterisl::PhysicsMaterisl() { COMP_LOG("Creating Physics Materisl"); }
  PhysicsMaterisl::~PhysicsMaterisl() { COMP_LOG("Destroying Physics Materisl"); }
  COMP_COPY_MOVE_CONSTRUCTORS(PhysicsMaterisl);
  void PhysicsMaterisl::Copy(const PhysicsMaterisl &other) {
    density = other.density;
    friction = other.friction;
    restitution = other.restitution;
    restitution_threshold = other.restitution_threshold;
  }
  
  void PhysicsMaterisl::RenderGui() {
    PropertyGrid::Float1("Density", density);
    PropertyGrid::Float1("Friction", friction);
    PropertyGrid::Float1("Restitution", restitution);
    PropertyGrid::Float1("Restitution Threshold", restitution_threshold);
  }
  
  // -------------------------------------------------------------------------
  // Box Colloider Component
  // -------------------------------------------------------------------------
  Box2DColliderComponent::Box2DColliderComponent() { COMP_LOG("Creating Box Collider Component"); }
  Box2DColliderComponent::~Box2DColliderComponent() { COMP_LOG("Destroying Box Collider Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(Box2DColliderComponent);
  void Box2DColliderComponent::Copy(const Box2DColliderComponent &other) {
    physics_mat = other.physics_mat;
    offset = other.offset;
    size = other.size;
    runtime_fixture = other.runtime_fixture;
  }
  
  void Box2DColliderComponent::RenderGui() {
    PropertyGrid::Float2("Offset", offset);
    PropertyGrid::Float2("Size", size);
    
    ImGui::Separator();
    physics_mat.RenderGui();
    ImGui::Separator();
    
    if (runtime_fixture) {
      Entity* e = (Entity*)runtime_fixture;
      PropertyGrid::ReadOnlyTextBox("Entity Name", e->GetComponent<TagComponent>().tag.c_str());
      ImGui::Separator();
    }
  }
  
  // -------------------------------------------------------------------------
  // Circle Colloider Component
  // -------------------------------------------------------------------------
  CircleColliiderComponent::CircleColliiderComponent() { COMP_LOG("Creating Circle Collider Component"); }
  CircleColliiderComponent::~CircleColliiderComponent() { COMP_LOG("Destroying Cirlce Collider Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(CircleColliiderComponent);
  void CircleColliiderComponent::Copy(const CircleColliiderComponent &other) {
    physics_mat = other.physics_mat;
    offset = other.offset;
    radius = other.radius;
    runtime_fixture = other.runtime_fixture;
  }

  void CircleColliiderComponent::RenderGui() {
    PropertyGrid::Float2("Offset", offset);
    PropertyGrid::Float1("Radius", radius);
    ImGui::Separator();
    physics_mat.RenderGui();
    ImGui::Separator();
    
    if (runtime_fixture) {
      Entity* e = (Entity*)runtime_fixture;
      PropertyGrid::ReadOnlyTextBox("Entity Name", e->GetComponent<TagComponent>().tag.c_str());
      ImGui::Separator();
    }
  }
  
  // -------------------------------------------------------------------------
  // Pill Box Colloider Component
  // -------------------------------------------------------------------------
  PillBoxColliderComponent::PillBoxColliderComponent() { COMP_LOG("Creating Pill Box Collider Component "); RecalculateColliders(); }
  PillBoxColliderComponent::~PillBoxColliderComponent() { COMP_LOG("Destroying Pill Box Collider Component "); }
  COMP_COPY_MOVE_CONSTRUCTORS(PillBoxColliderComponent);
  void PillBoxColliderComponent::Copy(const PillBoxColliderComponent &other) {
    reset_flag = other.reset_flag;
    offset = other.offset;
    width = other.width;
    height = other.height;
    bcc = other.bcc;
    top_ccc = other.top_ccc;
    bottom_ccc = other.bottom_ccc;
    RecalculateColliders();
  }
  
  void PillBoxColliderComponent::SetHeight(float height) {
    this->height = height;
    RecalculateColliders();
    reset_flag = true;
  }
  
  void PillBoxColliderComponent::SetWidth(float width) {
    this->width = width;
    RecalculateColliders();
    reset_flag = true;
  }
  
  void PillBoxColliderComponent::SetSize(const glm::vec2& size) {
    this->height = size.y;
    this->width = size.x;
    RecalculateColliders();
    reset_flag = true;
  }
  
  void PillBoxColliderComponent::RecalculateColliders() {
    float circle_radius = width;
    float box_height = std::max((height - circle_radius), 0.01f);
    
    top_ccc.radius = circle_radius;
    bottom_ccc.radius = circle_radius;
    
    top_ccc.offset = offset + glm::vec2(0, box_height);
    bottom_ccc.offset = offset - glm::vec2(0, box_height);
    
    bcc.size = {width - 0.03, box_height};
    bcc.offset = offset;
  }
  
  void PillBoxColliderComponent::RenderGui() {
    if (PropertyGrid::Float2("Offset", offset))
      RecalculateColliders();
    if (PropertyGrid::Float1("Width ", width))
      RecalculateColliders();
    if (PropertyGrid::Float1("Height ", height))
      RecalculateColliders();
    ImGui::Separator();
    
    static bool show_debug_colliders = false;
    PropertyGrid::CheckBox("Show Debug Collliders", show_debug_colliders);
    ImGui::Separator();

    if (show_debug_colliders) {
      const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth |
      ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_FramePadding;
      
      bool box_open = ImGui::TreeNodeEx("Box Collider", tree_node_flags);
      if (box_open) {
        bcc.RenderGui();
        ImGui::TreePop();
      }
      
      bool top_circle_open = ImGui::TreeNodeEx("Top Circle Collider", tree_node_flags);
      if (top_circle_open) {
        top_ccc.RenderGui();
        ImGui::TreePop();
      }
      
      bool bottom_circle_open = ImGui::TreeNodeEx("Bottom Circle Collider", tree_node_flags);
      if (bottom_circle_open) {
        bottom_ccc.RenderGui();
        ImGui::TreePop();
      }
    }
  }

  // -------------------------------------------------------------------------
  // Native Script Component
  // -------------------------------------------------------------------------
  NativeScriptComponent::NativeScriptComponent(std::string name, ScriptLoaderFn loader_fun) : script_name(name), loader_function(loader_fun) { COMP_LOG("Creating Native Script Compontnt"); }
  NativeScriptComponent::~NativeScriptComponent() { COMP_LOG("Destroying Native Script Compontnt"); }
  COMP_COPY_MOVE_CONSTRUCTORS(NativeScriptComponent);
  
  void NativeScriptComponent::Copy(const NativeScriptComponent &other) {
    loader_function = other.loader_function;
    script_name = other.script_name;
    
    ScriptManager::UpdateScript(this, script_name, loader_function);
    script->Copy((void*)other.script.get());
  }
  
  void NativeScriptComponent::RenderGui() {
    ImGui::PushID("Natiove Script Component");
    
    bool opened = ImGui::TreeNodeEx(script_name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen);
    if (opened) {
      if (script)
        script->RenderGui();
      ImGui::TreePop();
    }
    ImGui::PopID();
  }

  // -------------------------------------------------------------------------
  // TextComponent Component
  // -------------------------------------------------------------------------
  TextComponent::TextComponent() { COMP_LOG("Creating Text Component"); }
  TextComponent::~TextComponent() { COMP_LOG("Destroying Text Component"); }
  COMP_COPY_MOVE_CONSTRUCTORS(TextComponent);
  void TextComponent::Copy(const TextComponent& other) {
    text = other.text;
    color = other.color;
  }

  void TextComponent::RenderGui() {
    PropertyGrid::TextBox(text, "Text ");
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100.0f);
    ImGui::Text("Color");
    ImGui::NextColumn();
    ImGui::ColorEdit4("Color ", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Columns(1);
    ImGui::Separator();
  }
  
} // namespace ikan
