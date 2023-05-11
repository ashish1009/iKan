//
//  serialise_entity.cpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#include "serialise_entity.hpp"
#include <yaml-cpp/yaml.h>

namespace YAML {
  
  // yml converstions vec2
  template<> struct convert<glm::vec2> {
    static Node encode(const glm::vec2& rhs) {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.SetStyle(EmitterStyle::Flow);
      return node;
    }
    
    static bool decode(const Node& node, glm::vec2& rhs) {
      if (!node.IsSequence() or node.size() != 2)
        return false;
      
      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      return true;
    }
  };
  
  // yml converstions vec3
  template<> struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.push_back(rhs.z);
      node.SetStyle(EmitterStyle::Flow);
      return node;
    }
    
    static bool decode(const Node& node, glm::vec3& rhs) {
      if (!node.IsSequence() or node.size() != 3)
        return false;
      
      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      rhs.z = node[2].as<float>();
      return true;
    }
  };
  
  // yml converstions
  template<> struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.push_back(rhs.z);
      node.push_back(rhs.w);
      node.SetStyle(EmitterStyle::Flow);
      return node;
    }
    
    static bool decode(const Node& node, glm::vec4& rhs) {
      if (!node.IsSequence() or node.size() != 4)
        return false;
      
      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      rhs.z = node[2].as<float>();
      rhs.w = node[3].as<float>();
      return true;
    }
  };
  
} // namespace YAML


namespace ikan {

  // yml << operator for glm vec 3
  static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
  }
  // yml << operator for glm vec 2
  static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
  }
  // yml << operator for glm vec 4
  static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.a << YAML::EndSeq;
    return out;
  }
  
  void EntitySerialiser::SerializeBoxCollider(YAML::Emitter& out, const Box2DColliderComponent& bcc, std::string identifier) {
    out << YAML::Key << "Offset" + identifier << YAML::Value << bcc.offset;
    out << YAML::Key << "Size" + identifier << YAML::Value << bcc.size;
    out << YAML::Key << "Angle" + identifier << YAML::Value << bcc.angle;

    out << YAML::Key << "Density" + identifier << YAML::Value << bcc.physics_mat.density;
    out << YAML::Key << "Friction" + identifier << YAML::Value << bcc.physics_mat.friction;
    out << YAML::Key << "Restitution" + identifier << YAML::Value << bcc.physics_mat.restitution;
    out << YAML::Key << "Restitution Threshold" + identifier << YAML::Value << bcc.physics_mat.restitution_threshold;
  }
  
  void EntitySerialiser::DeserializeBoxCollider(Box2DColliderComponent& bcc, const YAML::Node& box_colloider_component, std::string identifier) {
    bcc.offset = box_colloider_component["Offset" + identifier].as<glm::vec2>();
    bcc.size = box_colloider_component["Size" + identifier].as<glm::vec2>();
    bcc.angle = box_colloider_component["Angle" + identifier].as<float>();

    bcc.physics_mat.density = box_colloider_component["Density" + identifier].as<float>();
    bcc.physics_mat.friction = box_colloider_component["Friction" + identifier].as<float>();
    bcc.physics_mat.restitution = box_colloider_component["Restitution" + identifier].as<float>();
    bcc.physics_mat.restitution_threshold = box_colloider_component["Restitution Threshold" + identifier].as<float>();
    
    IK_CORE_TRACE(LogModule::EntitySerializer, "    Box Collider Component");
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Offset                | {0} | {0}", bcc.offset.x, bcc.offset.y);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Size                  | {0} | {0}", bcc.size.x, bcc.size.y);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Density               | {0}", bcc.physics_mat.density);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Friction              | {0}", bcc.physics_mat.friction);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Restitution           | {0}", bcc.physics_mat.restitution);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Restitution Threshold | {0}", bcc.physics_mat.restitution_threshold);
  }
  
  void EntitySerialiser::SerializeCircleCollider(YAML::Emitter& out, const CircleColliiderComponent& ccc, std::string identifier) {
    out << YAML::Key << "Offset" + identifier << YAML::Value << ccc.offset;
    out << YAML::Key << "Radius" + identifier << YAML::Value << ccc.radius;
    
    out << YAML::Key << "Density" + identifier << YAML::Value << ccc.physics_mat.density;
    out << YAML::Key << "Friction" + identifier << YAML::Value << ccc.physics_mat.friction;
    out << YAML::Key << "Restitution" + identifier << YAML::Value << ccc.physics_mat.restitution;
    out << YAML::Key << "Restitution Threshold" + identifier << YAML::Value << ccc.physics_mat.restitution_threshold;
  }
  
  void EntitySerialiser::DeserializeCircleCollider(CircleColliiderComponent& ccc, const YAML::Node& circle_colloider_component, std::string identifier) {
    ccc.offset = circle_colloider_component["Offset" + identifier].as<glm::vec2>();
    ccc.radius = circle_colloider_component["Radius" + identifier].as<float>();
    
    ccc.physics_mat.density = circle_colloider_component["Density" + identifier].as<float>();
    ccc.physics_mat.friction = circle_colloider_component["Friction" + identifier].as<float>();
    ccc.physics_mat.restitution = circle_colloider_component["Restitution" + identifier].as<float>();
    ccc.physics_mat.restitution_threshold = circle_colloider_component["Restitution Threshold" + identifier].as<float>();
    
    IK_CORE_TRACE(LogModule::EntitySerializer, "    Circle Colloider Component");
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Offset                | {0} | {0}", ccc.offset.x, ccc.offset.y);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Radius                | {0}", ccc.radius);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Density               | {0}", ccc.physics_mat.density);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Friction              | {0}", ccc.physics_mat.friction);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Restitution           | {0}", ccc.physics_mat.restitution);
    IK_CORE_TRACE(LogModule::EntitySerializer, "      Restitution Threshold | {0}", ccc.physics_mat.restitution_threshold);
  }
  
  void EntitySerialiser::SerialiseEntity(YAML::Emitter& out, Entity entity) {
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<TagComponent>()) {
      out << YAML::Key << "TagComponent";
      out << YAML::BeginMap; // TagComponent
      
      auto& tag = entity.GetComponent<TagComponent>().tag;
      out << YAML::Key << "Tag" << YAML::Value << tag;
      
      out << YAML::EndMap; // TagComponent
    } // Tag Component
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<TransformComponent>()) {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap; // TransformComponent
      
      auto& tc = entity.GetComponent<TransformComponent>();
      out << YAML::Key << "Translation" << YAML::Value << tc.Position();
      out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation();
      out << YAML::Key << "Scale" << YAML::Value << tc.Scale();
      
      out << YAML::EndMap; // TransformComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<CameraComponent>()) {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap; // CameraComponent
      
      auto& cc = entity.GetComponent<CameraComponent>();
      out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.is_fixed_aspect_ratio;
      out << YAML::Key << "Primary" << YAML::Value << cc.is_primary;
      
      // Scene Camera
      const auto& camera = cc.camera;
      out << YAML::Key << "ProjectionType" << YAML::Value << (uint32_t)camera->GetProjectionType();
      out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera->GetPerspectiveFOV();
      out << YAML::Key << "OrthographicSize" << YAML::Value << camera->GetOrthographicSize();
      out << YAML::Key << "Near" << YAML::Value << camera->GetNear();
      out << YAML::Key << "Far" << YAML::Value << camera->GetFar();
      out << YAML::Key << "Grid" << YAML::Value << camera->grid_2d_;
      out << YAML::Key << "Isometric" << YAML::Value << camera->isometric_;

      out << YAML::EndMap; // CameraComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<QuadComponent>()) {
      out << YAML::Key << "QuadComponent";
      out << YAML::BeginMap; // QuadComponent
      
      auto& qc = entity.GetComponent<QuadComponent>();
      out << YAML::Key << "Texture_Use" << YAML::Value << qc.sprite.use;
      out << YAML::Key << "Sub_Texture_Use" << YAML::Value << qc.sprite.use_sub_texture;
      out << YAML::Key << "Linear_Edge" << YAML::Value << qc.sprite.linear_edge;
      
      if (qc.sprite.texture.size() > 0) {
        int32_t i = 0;
        for (;i < (int32_t)qc.sprite.texture.size(); i++) {
          std::string tag = "Texture_Path_" + std::to_string(i);
          out << YAML::Key << tag << YAML::Value << qc.sprite.texture.at(i)->GetfilePath();
        }
        out << YAML::Key << "Num_Texture_Path" << YAML::Value << i;
      }
      else {
        out << YAML::Key << "Num_Texture_Path" << YAML::Value << 0;
      }
      out << YAML::Key << "Texture_TilingFactor" << YAML::Value << qc.sprite.tiling_factor;
      out << YAML::Key << "Color" << YAML::Value << qc.color;
      
      out << YAML::Key << "Speed" << YAML::Value << qc.sprite.speed;
      
      int32_t num_sprite_coords = 0;
      for (const auto& sprite : qc.sprite.sprite_images) {
        out << YAML::Key << "Coords" + std::to_string(num_sprite_coords) << YAML::Value << sprite->GetCoords();
        out << YAML::Key << "Sprite_Size" + std::to_string(num_sprite_coords) << YAML::Value << sprite->GetSpriteSize();
        out << YAML::Key << "Cell_Size" + std::to_string(num_sprite_coords) << YAML::Value << sprite->GetCellSize();
        num_sprite_coords++;
      }
      out << YAML::Key << "Num_Coords" << YAML::Value << num_sprite_coords;
      
      out << YAML::EndMap; // QuadComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<CircleComponent>()) {
      out << YAML::Key << "CircleComponent";
      out << YAML::BeginMap; // CircleComponent
      
      auto& cc = entity.GetComponent<CircleComponent>();
      out << YAML::Key << "Texture_Use" << YAML::Value << cc.texture_comp.use;
      
      if (cc.texture_comp.texture.size() > 0) {
        int32_t i = 0;
        for (;i < (int32_t)cc.texture_comp.texture.size(); i++) {
          std::string tag = "Texture_Path_" + std::to_string(i);
          out << YAML::Key << tag << YAML::Value << cc.texture_comp.texture.at(i)->GetfilePath();
        }
        out << YAML::Key << "Num_Texture_Path" << YAML::Value << i;
      }
      else {
        out << YAML::Key << "Num_Texture_Path" << YAML::Value << 0;
      }
      
      out << YAML::Key << "Texture_TilingFactor" << YAML::Value << cc.texture_comp.tiling_factor;
      out << YAML::Key << "Color" << YAML::Value << cc.color;
      out << YAML::Key << "Thickness" << YAML::Value << cc.thickness;
      out << YAML::Key << "Fade" << YAML::Value << cc.fade;
      
      out << YAML::EndMap; // CircleComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<RigidBodyComponent>()) {
      out << YAML::Key << "RigidBodyComponent";
      out << YAML::BeginMap; // RigidBodyComponent
      
      auto& rc = entity.GetComponent<RigidBodyComponent>();
      
      uint32_t type = (uint32_t)rc.type;
      out << YAML::Key << "Type" << YAML::Value << type;
      out << YAML::Key << "Is Ground" << YAML::Value << rc.is_ground;
      out << YAML::Key << "Is Sensor" << YAML::Value << rc.is_sensor;
      out << YAML::Key << "Fixed Rotation" << YAML::Value << rc.fixed_rotation;
      
      out << YAML::Key << "Linear Velocity" << YAML::Value << rc.velocity;
      out << YAML::Key << "Angular Damping" << YAML::Value << rc.angular_damping;
      out << YAML::Key << "Angular Velocity" << YAML::Value << rc.angular_velocity;
      out << YAML::Key << "Linear Damping" << YAML::Value << rc.linear_damping;
      out << YAML::Key << "Gravity Scale" << YAML::Value << rc.gravity_scale;
      
      out << YAML::EndMap; // RigidBodyComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<Box2DColliderComponent>()) {
      out << YAML::Key << "BoxColloiderComponent";
      out << YAML::BeginMap; // BoxColloiderComponent
      
      auto& bcc = entity.GetComponent<Box2DColliderComponent>();
      SerializeBoxCollider(out, bcc, "");
      
      out << YAML::EndMap; // BoxColloiderComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<CircleColliiderComponent>()) {
      out << YAML::Key << "CircleColloiderComponent";
      out << YAML::BeginMap; // CircleColloiderComponent
      
      auto& ccc = entity.GetComponent<CircleColliiderComponent>();
      SerializeCircleCollider(out, ccc, "");
      
      out << YAML::EndMap; // CircleColloiderComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<PillBoxColliderComponent>()) {
      out << YAML::Key << "PillBoxColliderComponent";
      out << YAML::BeginMap; // PillBoxColliderComponent
      
      auto& pbc = entity.GetComponent<PillBoxColliderComponent>();
      
      out << YAML::Key << "Offset" << YAML::Value << pbc.offset;
      out << YAML::Key << "Width" << YAML::Value << pbc.width;
      out << YAML::Key << "Height" << YAML::Value << pbc.height;
      
      out << YAML::EndMap; // PillBoxColliderComponent
    }
    
    // ------------------------------------------------------------------------
    if (entity.HasComponent<NativeScriptComponent>()) {
      out << YAML::Key << "NativeScriptComponent";
      out << YAML::BeginMap; // NativeScriptComponent
      
      auto& sc = entity.GetComponent<NativeScriptComponent>();
      std::string name_tag = "Script_name_";
      out << YAML::Key << name_tag << YAML::Value << sc.script_name;
      
      out << YAML::EndMap; // NativeScriptComponent
    }
    
    out << YAML::EndMap; // Entity
  }
  
  void EntitySerialiser::DeserislizeEntity(YAML::Node entity, Entity deserialized_entity, Scene* scene_) {
    // --------------------------------------------------------------------
    auto transform_component = entity["TransformComponent"];
    if (transform_component) {
      // Entities always have transforms
      auto& tc = deserialized_entity.GetComponent<TransformComponent>();
      tc.UpdatePosition(transform_component["Translation"].as<glm::vec3>());
      tc.UpdateRotation(transform_component["Rotation"].as<glm::vec3>());
      tc.UpdateScale(transform_component["Scale"].as<glm::vec3>());
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Transform Component");
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Translation   | {0} | {1} | {2}", tc.Position().x, tc.Position().y, tc.Position().z);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Rotation      | {0} | {1} | {2}", tc.Rotation().x, tc.Rotation().y, tc.Rotation().z);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Scale         | {0} | {1} | {2}", tc.Scale().x, tc.Scale().y, tc.Scale().z);
    } // if (transform_component)
    
    // --------------------------------------------------------------------
    auto camera_component = entity["CameraComponent"];
    if (camera_component) {
      auto type = camera_component["ProjectionType"].as<uint32_t>();
      
      auto& cc = deserialized_entity.AddComponent<CameraComponent>((SceneCamera::ProjectionType)type);
      cc.is_fixed_aspect_ratio = camera_component["FixedAspectRatio"].as<bool>();
      cc.is_primary = camera_component["Primary"].as<bool>();
      
      auto fov = camera_component["PerspectiveFOV"].as<float>();
      auto size = camera_component["OrthographicSize"].as<float>();
      auto near = camera_component["Near"].as<float>();
      auto far = camera_component["Far"].as<float>();

      cc.camera->grid_2d_ = camera_component["Grid"].as<bool>();
      cc.camera->isometric_ = camera_component["Isometric"].as<bool>();
      
      if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic)
        cc.camera->SetOrthographic(size, near, far);
      else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective)
        cc.camera->SetPerspective(fov, near, far);
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Cameara Component");
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Primary             | {0}", cc.is_primary);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Fixed Aspect Ratio  | {0}", cc.is_fixed_aspect_ratio);
      
      if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic) {
        IK_CORE_TRACE(LogModule::EntitySerializer, "      Orthographic Camera");
        IK_CORE_TRACE(LogModule::EntitySerializer, "        Size | {0}", cc.camera->GetOrthographicSize());
      }
      else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective) {
        IK_CORE_TRACE(LogModule::EntitySerializer, "      Perspective Camera");
        IK_CORE_TRACE(LogModule::EntitySerializer, "        FOV  | {0}", cc.camera->GetPerspectiveFOV());
      }
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "         Near : {0}", cc.camera->GetNear());
      IK_CORE_TRACE(LogModule::EntitySerializer, "         Far  : {0}", cc.camera->GetFar());
    } // if (camera_component)
    
    // --------------------------------------------------------------------
    auto quad_component = entity["QuadComponent"];
    if (quad_component) {
      auto& qc = deserialized_entity.AddComponent<QuadComponent>();
      
      qc.sprite.use = quad_component["Texture_Use"].as<bool>();
      qc.sprite.use_sub_texture = quad_component["Sub_Texture_Use"].as<bool>();
      qc.sprite.linear_edge = quad_component["Linear_Edge"].as<bool>();
      
      int32_t num_tex = quad_component["Num_Texture_Path"].as<int32_t>();
      if (num_tex > 0) {
        for (int32_t i = 0; i < num_tex; i++) {
          std::string texture_path = quad_component["Texture_Path_" + std::to_string(i)].as<std::string>();
          if (texture_path != "") {
            qc.sprite.texture.push_back(Renderer::GetTexture(texture_path, qc.sprite.linear_edge));
          }
        }
      }
      qc.sprite.tiling_factor = quad_component["Texture_TilingFactor"].as<float>();
      qc.color = quad_component["Color"].as<glm::vec4>();
      
      qc.sprite.speed = quad_component["Speed"].as<int32_t>();
      
      int32_t num_coords = quad_component["Num_Coords"].as<int32_t>();
      for (int i = 0; i < num_coords; i++) {
        auto coord = quad_component["Coords" + std::to_string(i)].as<glm::vec2>();
        auto sprite_size = quad_component["Sprite_Size" + std::to_string(i)].as<glm::vec2>();
        auto cell_size = quad_component["Cell_Size" + std::to_string(i)].as<glm::vec2>();
        qc.sprite.sprite_images.push_back(SubTexture::CreateFromCoords(qc.sprite.texture.at(0), coord, sprite_size, cell_size));
      }
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Quad Component");
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Texture");
      IK_CORE_TRACE(LogModule::EntitySerializer, "        Use             | {0}", qc.sprite.use);
      if (qc.sprite.texture.size()) {
        for (auto& t : qc.sprite.texture)
          IK_CORE_TRACE(LogModule::EntitySerializer, "        Path            | {0}", t->GetfilePath());
      }
      else
        IK_CORE_TRACE(LogModule::EntitySerializer, "        No Texture      ");
      IK_CORE_TRACE(LogModule::EntitySerializer, "        Tiling Factor   | {0}", qc.sprite.tiling_factor);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Color | {0} | {1} | {2}", qc.color.x, qc.color.y, qc.color.z);
      
    } // if (quad_component)
    
    // --------------------------------------------------------------------
    auto circle_component = entity["CircleComponent"];
    if (circle_component) {
      auto& cc = deserialized_entity.AddComponent<CircleComponent>();
      
      cc.texture_comp.use = circle_component["Texture_Use"].as<bool>();
      
      std::string texture_path = circle_component["Texture_Path"].as<std::string>();
      int32_t num_tex = quad_component["Num_Texture_Path"].as<int32_t>();
      if (num_tex > 0) {
        for (int32_t i = 0; i < num_tex; i++) {
          std::string texture_path = quad_component["Texture_Path_" + std::to_string(i)].as<std::string>();
          if (texture_path != "") {
            cc.texture_comp.texture.push_back(Renderer::GetTexture(texture_path));
          }
        }
      }
      
      cc.texture_comp.tiling_factor = circle_component["Texture_TilingFactor"].as<float>();
      
      cc.color = circle_component["Color"].as<glm::vec4>();
      cc.thickness = circle_component["Thickness"].as<float>();
      cc.fade = circle_component["Fade"].as<float>();
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Circle Component");
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Texture");
      IK_CORE_TRACE(LogModule::EntitySerializer, "        Use             | {0}", cc.texture_comp.use);
      if (cc.texture_comp.texture.size()) {
        for (auto& t : cc.texture_comp.texture)
          IK_CORE_TRACE(LogModule::EntitySerializer, "        Path            | {0}", t->GetfilePath());
      }
      else
        IK_CORE_TRACE(LogModule::EntitySerializer, "        No Texture      ");
      IK_CORE_TRACE(LogModule::EntitySerializer, "        Tiling Factor   | {0}", cc.texture_comp.tiling_factor);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Color | {0} | {1} | {2}", cc.color.x, cc.color.y, cc.color.z);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Thickness         | {0}", cc.thickness);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Fade              | {0}", cc.fade);
    } // if (circle_component)
    
    // --------------------------------------------------------------------
    auto rigid_body_component = entity["RigidBodyComponent"];
    if (rigid_body_component) {
      auto& rc = deserialized_entity.AddComponent<RigidBodyComponent>();
      
      auto type = rigid_body_component["Type"].as<uint8_t>();
      rc.type = (RigidBodyComponent::RbBodyType)type;
      rc.fixed_rotation = rigid_body_component["Fixed Rotation"].as<bool>();
      rc.is_ground = rigid_body_component["Is Ground"].as<bool>();
      rc.is_sensor = rigid_body_component["Is Sensor"].as<bool>();

      rc.velocity         = rigid_body_component["Linear Velocity"].as<glm::vec2>();
      rc.angular_damping  = rigid_body_component["Angular Damping"].as<float>();
      rc.angular_velocity = rigid_body_component["Angular Velocity"].as<float>();
      rc.linear_damping   = rigid_body_component["Linear Damping"].as<float>();
      rc.gravity_scale    = rigid_body_component["Gravity Scale"].as<float>();
      
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Rigid Body Component");
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Type             | {0}", type);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Fixed Rotation   | {0}", rc.fixed_rotation);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Is Ground        | {0}", rc.is_ground);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Is Sensor        | {0}", rc.is_sensor);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Linear Velocity  | {0} | {1}", rc.velocity.x, rc.velocity.y);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Angular Velocity | {0}", rc.angular_velocity);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Linear Damping   | {0}", rc.linear_damping);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Angular Damping  | {0}", rc.angular_damping);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Gravity Scale    | {0}", rc.gravity_scale);
    } // if (rigid_body_component)
    
    // --------------------------------------------------------------------
    auto box_colloider_component = entity["BoxColloiderComponent"];
    if (box_colloider_component) {
      auto& bcc = deserialized_entity.AddComponent<Box2DColliderComponent>();
      DeserializeBoxCollider(bcc, box_colloider_component, "");
    } // if (box_colloider_component)
    
    // --------------------------------------------------------------------
    auto circle_colloider_component = entity["CircleColloiderComponent"];
    if (circle_colloider_component) {
      auto& ccc = deserialized_entity.AddComponent<CircleColliiderComponent>();
      DeserializeCircleCollider(ccc, circle_colloider_component, "");
    } // if (circle_colloider_component)
    
    // --------------------------------------------------------------------
    auto Pill_box_colloider_component = entity["PillBoxColliderComponent"];
    if (Pill_box_colloider_component) {
      auto& pbc = deserialized_entity.AddComponent<PillBoxColliderComponent>();
      
      pbc.offset = Pill_box_colloider_component["Offset"].as<glm::vec2>();
      pbc.width  = Pill_box_colloider_component["Width"].as<float>();
      pbc.height = Pill_box_colloider_component["Height"].as<float>();
      
      pbc.RecalculateColliders();
    } // if (Pill_box_colloider_component)
    
    // --------------------------------------------------------------------
    auto script_component = entity["NativeScriptComponent"];
    if (script_component) {
      std::string name_tag = "Script_name_";
      IK_CORE_TRACE(LogModule::EntitySerializer, "    Script Component");
      std::string script_name = script_component[name_tag].as<std::string>();
      
      auto& sc = deserialized_entity.AddComponent<NativeScriptComponent>(script_name);
      ScriptManager::UpdateScript(&sc, script_name, sc.loader_function);
      IK_CORE_TRACE(LogModule::EntitySerializer, "      Script | {0}", script_name);
    } // if (script_component)
  }
  
} // namespace ikan
