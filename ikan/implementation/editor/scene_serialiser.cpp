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
    IK_CORE_INFO(LogModule::SceneSerializer, "Serialising a Scene");
    IK_CORE_INFO(LogModule::SceneSerializer, "  Path | {0}", file_path);
    IK_CORE_INFO(LogModule::SceneSerializer, "  Name | {0}", scene_->name_);
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    
    std::ofstream fout(file_path);
    fout << out.c_str();
  }

  bool SceneSerializer::Deserialize(const std::string& file_path) {
    return true;
  }
  
} // namespace ikan
