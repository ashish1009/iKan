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
  }

  bool SceneSerializer::Deserialize(const std::string& file_path) {
    return true;
  }
  
} // namespace ikan
