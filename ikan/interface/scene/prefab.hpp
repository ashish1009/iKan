//
//  prefab_serialiser.hpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#pragma once

#include "scene/core_entity.hpp"

namespace ikan {
  
  static const std::string prefab_extenstion_ = ".ikanPrefab";
  
  class Texture;
  
  struct ImageData {
    bool has_data = false;
    std::shared_ptr<Texture> texture;
    glm::vec2 uv0 = glm::vec2(0.0f, 1.0f), uv1 = glm::vec2(1.0f, 0.0f);
  };
  
  class Prefab {
  public:
    /// This functions serializes(Saves) the scene at path 'file_path'
    /// - Parameters:
    ///   - entity: Entity to be serialised
    ///   - file_path: path wehre scene need to be saved
    static void Serialize(Entity* entity, const std::string& file_path);
    /// This functions serializes(Opens) the scene frol path 'file_path'
    /// - Parameters:
    ///   - file_path: path which need to be loaded
    ///   - scene: Active Scene
    static Entity Deserialize(const std::string& file_path, Scene* scene);
    /// This Funciton Rneders a GUI Widget to load prefab
    /// - Parameter flag: flag to hide the widget
    static void Loader(bool* flag);
    
    static ImageData GetImageData(const std::string& file_path);
  };
  
} // namespace ikan
