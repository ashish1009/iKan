//
//  game_data.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#include "game_data.hpp"
#include "editor.hpp"
#include "mario.hpp"
#include "chess.hpp"
#include "angry_bird.hpp"

namespace kreator {
  
  std::unique_ptr<GameData> CreateGameData(GameType game_type) {
    switch (game_type) {
      case GameType::Mario: return std::make_unique<mario::Mario>();
      case GameType::Chess: return std::make_unique<chess::Chess>();
      case GameType::AngryBird: return std::make_unique<angry_bird::AngryBird>();
      case GameType::Editor:
      default:
        return std::make_unique<editor::Editor>();
    }
  }
  
  void GameData::LoadPrefab(const std::string& path, const Viewport& viewport) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;

    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport.height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport.mouse_pos_x - (float)viewport.width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport.mouse_pos_y - (float)viewport.height / 2) / zoom) + cam_data.position.y);
      
      Entity e = Prefab::Deserialize(path, scene_.get());
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
    }
  }
  
} // namespace kreator
