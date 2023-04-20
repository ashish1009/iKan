//
//  scene_serialiser.cpp
//  ikan
//
//  Created by Ashish . on 12/04/23.
//

#include "scene_serialiser.hpp"
#include "core_entity.hpp"
#include "components.hpp"
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
  
  static void SerializeBoxCollider(YAML::Emitter& out, const Box2DColliderComponent& bcc, std::string identifier) {
    out << YAML::Key << "Offset" + identifier << YAML::Value << bcc.offset;
    out << YAML::Key << "Size" + identifier << YAML::Value << bcc.size;
    
    out << YAML::Key << "Density" + identifier << YAML::Value << bcc.physics_mat.density;
    out << YAML::Key << "Friction" + identifier << YAML::Value << bcc.physics_mat.friction;
    out << YAML::Key << "Restitution" + identifier << YAML::Value << bcc.physics_mat.restitution;
    out << YAML::Key << "Restitution Threshold" + identifier << YAML::Value << bcc.physics_mat.restitution_threshold;
  }
  
  static void DeserializeBoxCollider(Box2DColliderComponent& bcc, const YAML::Node& box_colloider_component, std::string identifier) {
    bcc.offset = box_colloider_component["Offset" + identifier].as<glm::vec2>();
    bcc.size = box_colloider_component["Size" + identifier].as<glm::vec2>();
    
    bcc.physics_mat.density = box_colloider_component["Density" + identifier].as<float>();
    bcc.physics_mat.friction = box_colloider_component["Friction" + identifier].as<float>();
    bcc.physics_mat.restitution = box_colloider_component["Restitution" + identifier].as<float>();
    bcc.physics_mat.restitution_threshold = box_colloider_component["Restitution Threshold" + identifier].as<float>();
    
    IK_CORE_TRACE(LogModule::SceneSerializer, "    Box Collider Component");
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Offset                | {0} | {0}", bcc.offset.x, bcc.offset.y);
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Size                  | {0} | {0}", bcc.size.x, bcc.size.y);
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Density               | {0}", bcc.physics_mat.density);
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Friction              | {0}", bcc.physics_mat.friction);
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Restitution           | {0}", bcc.physics_mat.restitution);
    IK_CORE_TRACE(LogModule::SceneSerializer, "      Restitution Threshold | {0}", bcc.physics_mat.restitution_threshold);
  }

  SceneSerializer::SceneSerializer(Scene* scene) : scene_(scene) { }
  SceneSerializer::~SceneSerializer() { }
  
  void SceneSerializer::Serialize(const std::string& file_path) {
    IK_CORE_TRACE(LogModule::SceneSerializer, "Serialising a Scene");
    IK_CORE_TRACE(LogModule::SceneSerializer, "  Path  {0}", file_path);
    IK_CORE_TRACE(LogModule::SceneSerializer, "  Name  {0}", scene_->name_);
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Scene_type" << YAML::Value << (uint32_t)scene_->type_;

    // Scene Debugger data
    {
      out << YAML::Key << "Setting_editor_camera" << YAML::Value << scene_->setting_.editor_camera;
      out << YAML::Key << "Setting_use_editor_camera" << YAML::Value << scene_->setting_.use_editor_camera;
    }

    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    // Serialize the scene for all the entities
    for (const auto& [entt_id, entity] : scene_->entity_id_map_) {
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
        
        out << YAML::EndMap; // CameraComponent
      }
      
      // ------------------------------------------------------------------------
      if (entity.HasComponent<QuadComponent>()) {
        out << YAML::Key << "QuadComponent";
        out << YAML::BeginMap; // QuadComponent
        
        auto& qc = entity.GetComponent<QuadComponent>();
        out << YAML::Key << "Texture_Use" << YAML::Value << qc.sprite.use;
        out << YAML::Key << "Sprite_Use" << YAML::Value << qc.sprite.use_sub_texture;
        out << YAML::Key << "Linear_Edge" << YAML::Value << qc.sprite.linear_edge;
        
        if (qc.sprite.texture) {
          out << YAML::Key << "Texture_Path" << YAML::Value << qc.sprite.texture->GetfilePath();
          out << YAML::Key << "Coords" << YAML::Value << qc.sprite.sub_texture->GetCoords();
          out << YAML::Key << "Sprite_Size" << YAML::Value << qc.sprite.sub_texture->GetSpriteSize();
          out << YAML::Key << "Cell_Size" << YAML::Value << qc.sprite.sub_texture->GetCellSize();
        }
        else {
          out << YAML::Key << "Texture_Path" << YAML::Value << "";
        }
        out << YAML::Key << "Texture_TilingFactor" << YAML::Value << qc.sprite.tiling_factor;
        out << YAML::Key << "Color" << YAML::Value << qc.color;
        
        out << YAML::EndMap; // QuadComponent
      }
      
      // ------------------------------------------------------------------------
      if (entity.HasComponent<CircleComponent>()) {
        out << YAML::Key << "CircleComponent";
        out << YAML::BeginMap; // CircleComponent
        
        auto& cc = entity.GetComponent<CircleComponent>();
        out << YAML::Key << "Texture_Use" << YAML::Value << cc.texture_comp.use;
        
        if (cc.texture_comp.texture)
          out << YAML::Key << "Texture_Path" << YAML::Value << cc.texture_comp.texture->GetfilePath();
        else
          out << YAML::Key << "Texture_Path" << YAML::Value << "";
        
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

      out << YAML::EndMap; // Entity
    } // // for (const auto& [uuid, entity] : scene_->entity_id_map_)
    
    std::ofstream fout(file_path);
    fout << out.c_str();
  }

  bool SceneSerializer::Deserialize(const std::string& file_path) {
    if (file_path == "" or StringUtils::GetExtensionFromFilePath(file_path) != saved_scene_extension_ ) return false;

    YAML::Node data = YAML::LoadFile(file_path);

    if (!data["Scene"])
      return false;

    std::string scene_name = data["Scene"].as<std::string>();
    IK_CORE_TRACE(LogModule::SceneSerializer, "Deserialising scene");
    IK_CORE_TRACE(LogModule::SceneSerializer, "  Path  {0}", file_path);
    IK_CORE_TRACE(LogModule::SceneSerializer, "  Name  {0}", scene_name);
    
    scene_->type_ = Scene::Type(data["Scene_type"].as<uint32_t>());
    
    // Scene Debugger
    scene_->setting_.editor_camera = data["Setting_editor_camera"].as<bool>();
    scene_->setting_.use_editor_camera = data["Setting_use_editor_camera"].as<bool>();

    auto entities = data["Entities"];
    if (entities) {
      for (auto entity : entities) {
        uint64_t uuid = entity["Entity"].as<uint64_t>();

        std::string name;
        auto tag_component = entity["TagComponent"];
        name = tag_component["Tag"].as<std::string>();

        Entity deserialized_entity = scene_->CreateEntity(name, uuid);
        IK_CORE_TRACE(LogModule::SceneSerializer, "  Deserialising Entity");
        IK_CORE_TRACE(LogModule::SceneSerializer, "  ID     {0}", uuid);
        IK_CORE_TRACE(LogModule::SceneSerializer, "  Name   {0}", name);
        
        // --------------------------------------------------------------------
        auto transform_component = entity["TransformComponent"];
        if (transform_component) {
          // Entities always have transforms
          auto& tc = deserialized_entity.GetComponent<TransformComponent>();
          tc.UpdatePosition(transform_component["Translation"].as<glm::vec3>());
          tc.UpdateRotation(transform_component["Rotation"].as<glm::vec3>());
          tc.UpdateScale(transform_component["Scale"].as<glm::vec3>());
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "    Transform Component");
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Translation   | {0} | {1} | {2}", tc.Position().x, tc.Position().y, tc.Position().z);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Rotation      | {0} | {1} | {2}", tc.Rotation().x, tc.Rotation().y, tc.Rotation().z);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Scale         | {0} | {1} | {2}", tc.Scale().x, tc.Scale().y, tc.Scale().z);
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
          
          if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic)
            cc.camera->SetOrthographic(size, near, far);
          else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective)
            cc.camera->SetPerspective(fov, near, far);
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "    Cameara Component");
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Primary             | {0}", cc.is_primary);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Fixed Aspect Ratio  | {0}", cc.is_fixed_aspect_ratio);
          
          if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Orthographic) {
            IK_CORE_TRACE(LogModule::SceneSerializer, "      Orthographic Camera");
            IK_CORE_TRACE(LogModule::SceneSerializer, "        Size | {0}", cc.camera->GetOrthographicSize());
          }
          else if ((SceneCamera::ProjectionType)type == SceneCamera::ProjectionType::Perspective) {
            IK_CORE_TRACE(LogModule::SceneSerializer, "      Perspective Camera");
            IK_CORE_TRACE(LogModule::SceneSerializer, "        FOV  | {0}", cc.camera->GetPerspectiveFOV());
          }
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "         Near : {0}", cc.camera->GetNear());
          IK_CORE_TRACE(LogModule::SceneSerializer, "         Far  : {0}", cc.camera->GetFar());
        } // if (camera_component)

        // --------------------------------------------------------------------
        auto quad_component = entity["QuadComponent"];
        if (quad_component) {
          auto& qc = deserialized_entity.AddComponent<QuadComponent>();
          
          qc.sprite.use = quad_component["Texture_Use"].as<bool>();
          qc.sprite.use_sub_texture = quad_component["Sprite_Use"].as<bool>();
          qc.sprite.linear_edge = quad_component["Linear_Edge"].as<bool>();
          
          std::string texture_path = quad_component["Texture_Path"].as<std::string>();
          if (texture_path != "") {
            qc.sprite.texture = Renderer::GetTexture(texture_path, qc.sprite.linear_edge);
            
            const glm::vec2& coords = quad_component["Coords"].as<glm::vec2>();
            const glm::vec2& sprite_size = quad_component["Sprite_Size"].as<glm::vec2>();
            const glm::vec2& cell_size = quad_component["Cell_Size"].as<glm::vec2>();
            qc.sprite.sub_texture = SubTexture::CreateFromCoords(qc.sprite.texture, coords, sprite_size, cell_size);
          }
          
          qc.sprite.tiling_factor = quad_component["Texture_TilingFactor"].as<float>();
          qc.color = quad_component["Color"].as<glm::vec4>();
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "    Quad Component");
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Texture");
          IK_CORE_TRACE(LogModule::SceneSerializer, "        Use             | {0}", qc.sprite.use);
          if (qc.sprite.texture)
            IK_CORE_TRACE(LogModule::SceneSerializer, "        Path            | {0}", qc.sprite.texture->GetfilePath());
          else
            IK_CORE_TRACE(LogModule::SceneSerializer, "        No Texture      ");
          IK_CORE_TRACE(LogModule::SceneSerializer, "        Tiling Factor   | {0}", qc.sprite.tiling_factor);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Color | {0} | {1} | {2}", qc.color.x, qc.color.y, qc.color.z);
          
        } // if (quad_component)
        
        // --------------------------------------------------------------------
        auto circle_component = entity["CircleComponent"];
        if (circle_component) {
          auto& cc = deserialized_entity.AddComponent<CircleComponent>();
          
          cc.texture_comp.use = circle_component["Texture_Use"].as<bool>();
          
          std::string texture_path = circle_component["Texture_Path"].as<std::string>();
          if (texture_path != "")
            cc.texture_comp.texture = Renderer::GetTexture(texture_path);
          
          cc.texture_comp.tiling_factor = circle_component["Texture_TilingFactor"].as<float>();
          
          cc.color = circle_component["Color"].as<glm::vec4>();
          cc.thickness = circle_component["Thickness"].as<float>();
          cc.fade = circle_component["Fade"].as<float>();
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "    Circle Component");
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Texture");
          IK_CORE_TRACE(LogModule::SceneSerializer, "        Use             | {0}", cc.texture_comp.use);
          if (cc.texture_comp.texture)
            IK_CORE_TRACE(LogModule::SceneSerializer, "        Path            | {0}", cc.texture_comp.texture->GetfilePath());
          else
            IK_CORE_TRACE(LogModule::SceneSerializer, "        No Texture      ");
          IK_CORE_TRACE(LogModule::SceneSerializer, "        Tiling Factor   | {0}", cc.texture_comp.tiling_factor);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Color | {0} | {1} | {2}", cc.color.x, cc.color.y, cc.color.z);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Thickness         | {0}", cc.thickness);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Fade              | {0}", cc.fade);
        } // if (circle_component)
        
        // --------------------------------------------------------------------
        auto rigid_body_component = entity["RigidBodyComponent"];
        if (rigid_body_component) {
          auto& rc = deserialized_entity.AddComponent<RigidBodyComponent>();
          
          auto type = rigid_body_component["Type"].as<uint8_t>();
          rc.type = (RigidBodyComponent::RbBodyType)type;
          rc.fixed_rotation = rigid_body_component["Fixed Rotation"].as<bool>();
          rc.is_ground = rigid_body_component["Is Ground"].as<bool>();
          
          rc.velocity         = rigid_body_component["Linear Velocity"].as<glm::vec2>();
          rc.angular_damping  = rigid_body_component["Angular Damping"].as<float>();
          rc.angular_velocity = rigid_body_component["Angular Velocity"].as<float>();
          rc.linear_damping   = rigid_body_component["Linear Damping"].as<float>();
          rc.gravity_scale    = rigid_body_component["Gravity Scale"].as<float>();
          
          IK_CORE_TRACE(LogModule::SceneSerializer, "    Rigid Body Component");
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Type             | {0}", type);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Fixed Rotation   | {0}", rc.fixed_rotation);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Is Ground        | {0}", rc.is_ground);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Linear Velocity  | {0} | {1}", rc.velocity.x, rc.velocity.y);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Angular Velocity | {0}", rc.angular_velocity);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Linear Damping   | {0}", rc.linear_damping);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Angular Damping  | {0}", rc.angular_damping);
          IK_CORE_TRACE(LogModule::SceneSerializer, "      Gravity Scale    | {0}", rc.gravity_scale);
        } // if (rigid_body_component)

        // --------------------------------------------------------------------
        auto box_colloider_component = entity["BoxColloiderComponent"];
        if (box_colloider_component) {
          auto& bcc = deserialized_entity.AddComponent<Box2DColliderComponent>();
          DeserializeBoxCollider(bcc, box_colloider_component, "");
        } // if (box_colloider_component)

      } // for (auto entity : entities)
    } // if (entities)
    
    return true;
  }
  
} // namespace ikan
