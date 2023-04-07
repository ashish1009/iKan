//
//  camera.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

namespace ikan {
  
  /// This class is the Base class of camera. Both the cameras should derived from this base class
  class Camera {
  public:
    /// Default Camera constructor
    /// - Parameters:
    ///   - aspect_ratio: Aspect ratio of the famera
    ///   - near_plane: Near plane of camera
    ///   - far_plane: far plane of camera
    Camera(float aspect_ratio = 16.0f / 9.0f, float near_plane = 0.0f, float far_plane = 0.0f)
    : aspect_ratio_(aspect_ratio), near_plane_(near_plane), far_plane_(far_plane) {}

    /// Default Camera destructor
    virtual ~Camera() = default;
    
    /// This function updates the Near plane of camera
    /// - Parameter near: New near plane
    virtual void SetNear(float near) { near_plane_ = near; }
    /// This function updates the Far plane of camera
    /// - Parameter far: new fat plane
    virtual void SetFar(float far) { far_plane_ = far;}
    
    /// This function returns the zoom value of camera
    virtual float GetZoom() const = 0;
    
    /// This function returns the Near Plane value
    float GetNear() const { return near_plane_; }
    /// This function returns the Far Plane value
    float GetFar() const { return far_plane_; }
    /// This function returns the Aspect Ratio value
    float GetAspectRatio() const { return aspect_ratio_; }
    /// This function returns the Projection Matrix of Camera
    const glm::mat4& GetProjection() const { return projection_matrix_; }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Camera);
    
  protected:
    float near_plane_ = 0.0f;
    float far_plane_ = 0.0f;
    float aspect_ratio_ = 16.0f / 9.0f;
    
    glm::mat4 projection_matrix_ = glm::mat4(1.0f);
  };
  
} // namespace ikan
