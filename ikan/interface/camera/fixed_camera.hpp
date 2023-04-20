//
//  fixed_camera.hpp
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#pragma once

#include "camera/camera.hpp"

namespace ikan {
  
  class FixedCamera : public Camera {
  public:
    /// This constructor creates the Scene Camera instance
    FixedCamera();
    /// Default Scene Camera Desctructorr
    virtual ~FixedCamera();

    /// This function returns the zoom value of camera
    float GetZoom() const override { return orthographic_size_; }

    /// This function updates the Viewport size of camera
    /// - Parameters:
    ///   - width: new width
    ///   - height: new height
    void SetViewportSize(uint32_t width, uint32_t height);

    /// This function renders Imgui pannel for Scene should cal this function between Imgui::Begin and Imgui::End
    /// - Parameter flag: flag to enable widget
    void RenderGui(bool* flag);

  private:
    /// This function recalculate the projection matrix
    void RecalculateProjection();

    float orthographic_size_ = 10.0f;
  };
  
} // namespace ikan
