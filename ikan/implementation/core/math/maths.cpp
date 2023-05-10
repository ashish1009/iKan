//
//  maths.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "maths.hpp"

namespace ikan {
  
  void Math::DecomposeTransform(const glm::mat4& transform, glm::vec3& out_translation, glm::vec3& out_rotation, glm::vec3& out_scale) {
    // From glm::decompose in matrix_decompose.inl
    using namespace glm;
    using T = float;
    
    mat4 LocalMatrix(transform);
    
    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
      return;
    
    // First, isolate perspective.  This is the messiest.
    if ( epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) or
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) or
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
      // Clear the perspective partition
      LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
      LocalMatrix[3][3] = static_cast<T>(1);
    }
    
    // Next take care of translation (easy).
    out_translation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);
    
    vec3 Row[3];
    
    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
      for (length_t j = 0; j < 3; ++j)
        Row[i][j] = LocalMatrix[i][j];
    
    // Compute X scale factor and normalize first row.
    out_scale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    out_scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    out_scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));
    
    out_rotation.y = asin(-Row[0][2]);
    if (cos(out_rotation.y) != 0) {
      out_rotation.x = atan2(Row[1][2], Row[2][2]);
      out_rotation.z = atan2(Row[0][1], Row[0][0]);
    } else {
      out_rotation.x = atan2(-Row[2][0], Row[1][1]);
      out_rotation.z = 0;
    }
  }
  
  glm::mat4 Math::GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotaiton, const glm::vec3& scale) {
    glm::mat4 rotation = glm::toMat4(glm::quat(rotaiton));
    return glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
  }
  
  glm::vec2 Math::GetIsometricFromCartesian(const glm::vec2 &cartisian) {
    return {cartisian.x - cartisian.y, (cartisian.x + cartisian.y) / 2};
  }
  
} // namespace ikan
