//
//  fixed_camera.cpp
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#include "fixed_camera.hpp"
#include "property_grid.hpp"

namespace ikan {
    
  FixedCamera::FixedCamera() {
    IK_CORE_TRACE(LogModule::FixedCamera, "Creating Fixed Camera ...");

    far_plane_ = 1.0f;
    near_plane_ = -1.0f;
      
    IK_CORE_TRACE(LogModule::FixedCamera, "  Size             {0}", orthographic_size_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Far Plane        {0}", far_plane_);
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  FixedCamera::~FixedCamera() {
    IK_CORE_TRACE(LogModule::FixedCamera, "Destroying Scene Camera !!!");
    IK_CORE_TRACE(LogModule::FixedCamera, "  Size             {0}", orthographic_size_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::FixedCamera, "  Far Plane        {0}", far_plane_);
  }
  
  void FixedCamera::RecalculateProjection() {
    float ortho_left = -orthographic_size_ * aspect_ratio_ * 0.5f;
    float ortho_right = orthographic_size_ * aspect_ratio_ * 0.5f;
    float ortho_bottom = -orthographic_size_ * 0.5f;
    float ortho_top = orthographic_size_ * 0.5f;
    
    projection_matrix_ = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, near_plane_, far_plane_);
  }
  
  void FixedCamera::SetViewportSize(uint32_t width, uint32_t height) {
    aspect_ratio_ = (float)width / (float)height;

    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void FixedCamera::RenderGui() {
    ImGui::Begin("Fixed Camera");
    ImGui::PushID("Fixed Camera");

    if (PropertyGrid::Float1("Size", orthographic_size_, nullptr, 1.0f, 10.0f)) {
      RecalculateProjection();
    }
    
    ImGui::Separator();
    ImGui::PopID();
    ImGui::End();
  }
  
} // namespace ikan
