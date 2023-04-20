//
//  fixed_camera.hpp
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#pragma once

namespace ikan {
  
  /// This structure stores the fixed projection of camera
  /// - Note: Camera origin at bottm left corner. And Its orthographic size is too much (Objects will be smaller in size)
  struct FixedCamera {
    static void SetViewport(float width, float height) { projection = glm::ortho( 0.0f, (float)width, 0.0f, (float)height); }
    inline static glm::mat4 projection;
  };
  
} // namespace ikan
