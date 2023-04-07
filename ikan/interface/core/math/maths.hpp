//
//  maths.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

#include <glm/glm.hpp>

namespace ikan {
  
  class Math {
  public:
    /// This funciton decomposes the Transform matrix as position rotation and scale
    /// - Parameters:
    ///   - transform: Transfrom matrix to be decomposed:
    ///   - out_translation: Position output
    ///   - out_rotation: Rotation output
    ///   - out_scale: Scale output
    ///  - Important: This function cost too much in CPU
    static void DecomposeTransform(const glm::mat4& transform, glm::vec3& out_translation, glm::vec3& out_rotation, glm::vec3& out_scale);
    
    /// This function generates and returns the transform matrix from position scale and rotation
    /// - Parameters:
    ///   - position: Position of the component:
    ///   - rotation: Rotation of the component. Default glm vec3(0.0f):
    ///   - scale: Size of the component. Default glm vec3(1.0f):
    ///  - Important: Return value should not be discarded.
    ///  - Important: This function cost too much in CPU
    [[nodiscard]] static glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    MAKE_PURE_STATIC(Math);
  };
  
} // namespace ikan
