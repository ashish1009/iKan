//
//  scene_serialiser.cpp
//  ikan
//
//  Created by Ashish . on 12/04/23.
//

#include "scene_serialiser.hpp"

#include <yaml-cpp/yaml.h>

namespace YAML {
  
} // namespace YAML


namespace ikan {
  
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

    return true;
  }
  
} // namespace ikan
