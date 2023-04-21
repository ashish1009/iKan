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
  
  class Prefab {
  public:
    /// This functions serializes(Saves) the scene at path 'file_path'
    /// - Parameters:
    ///   - entity: Entity to be serialised
    ///   - file_path: path wehre scene need to be saved
    static void Serialize(Entity* entity, const std::string& file_path);
    /// This functions serializes(Opens) the scene frol path 'file_path'
    /// - Parameter file_path: path which need to be loaded
    static Entity* Deserialize(const std::string& file_path);
    /// This Funciton Rneders a GUI Widget to load prefab
    /// - Parameter flag: flag to hide the widget
    static void Loader(bool* flag);
    
  };
  
} // namespace ikan
