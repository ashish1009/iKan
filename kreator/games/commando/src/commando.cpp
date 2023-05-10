//
//  commando.cpp
//  kreator
//
//  Created by Ashish . on 08/05/23.
//

#include "commando.hpp"

namespace commando {
  
  void Commando::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    scene_ = scene;
    viewport_ = viewport;
  };

  void Commando::AddQuadFromTexture(const std::string& path) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_->height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport_->mouse_pos_x - (float)viewport_->width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport_->mouse_pos_y - (float)viewport_->height / 2) / zoom) + cam_data.position.y);

      x_pos += 0.5f; // Adding 0.5 offset as camera starts from 0
      y_pos += 0.5f; // Adding 0.5 offset as camera starts from 0

      x_pos = std::floor(x_pos);
      y_pos = std::floor(y_pos);

      std::string name = StringUtils::GetNameFromFilePath(path);
      Entity e = scene_->CreateEntity(name);
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
      
      auto& qc = e.AddComponent<QuadComponent>();
      qc.sprite.texture.push_back(Renderer::GetTexture(path));
    }
  }
  
}
