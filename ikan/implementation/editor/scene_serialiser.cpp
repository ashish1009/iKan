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
          
          IK_CORE_INFO(LogModule::SceneSerializer, "    Transform Component");
          IK_CORE_INFO(LogModule::SceneSerializer, "      Translation   | {0} | {1} | {2}", tc.Position().x, tc.Position().y, tc.Position().z);
          IK_CORE_INFO(LogModule::SceneSerializer, "      Rotation      | {0} | {1} | {2}", tc.Rotation().x, tc.Rotation().y, tc.Rotation().z);
          IK_CORE_INFO(LogModule::SceneSerializer, "      Scale         | {0} | {1} | {2}", tc.Scale().x, tc.Scale().y, tc.Scale().z);
        } // if (transform_component)
        
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
          
          IK_CORE_INFO(LogModule::SceneSerializer, "    Quad Component");
          IK_CORE_INFO(LogModule::SceneSerializer, "      Texture");
          IK_CORE_INFO(LogModule::SceneSerializer, "        Use             | {0}", qc.sprite.use);
          if (qc.sprite.texture)
            IK_CORE_INFO(LogModule::SceneSerializer, "        Path            | {0}", qc.sprite.texture->GetfilePath());
          else
            IK_CORE_INFO(LogModule::SceneSerializer, "        No Texture      ");
          IK_CORE_INFO(LogModule::SceneSerializer, "        Tiling Factor   | {0}", qc.sprite.tiling_factor);
          IK_CORE_INFO(LogModule::SceneSerializer, "      Color | {0} | {1} | {2}", qc.color.x, qc.color.y, qc.color.z);
          
        } // if (quad_component)
      } // for (auto entity : entities)
    } // if (entities)
    
    return true;
  }
  
} // namespace ikan
