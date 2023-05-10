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
#include "commando.hpp"

namespace kreator {
  
  std::unique_ptr<GameData> CreateGameData(GameType game_type) {
    switch (game_type) {
      case GameType::Mario: return std::make_unique<mario::Mario>();
      case GameType::Chess: return std::make_unique<chess::Chess>();
      case GameType::AngryBird: return std::make_unique<angry_bird::AngryBird>();
      case GameType::Commando: return std::make_unique<commando::Commando>();
      case GameType::Editor:
      default:
        return std::make_unique<editor::Editor>();
    }
  }
  
  void GameData::LoadPrefab(const std::string& path) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;

    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_->height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport_->mouse_pos_x - (float)viewport_->width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport_->mouse_pos_y - (float)viewport_->height / 2) / zoom) + cam_data.position.y);
      
      Entity e = Prefab::Deserialize(path, scene_.get());
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
    }
  }
  
  void GameData::AddQuadFromTexture(const std::string& path) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_->height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport_->mouse_pos_x - (float)viewport_->width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport_->mouse_pos_y - (float)viewport_->height / 2) / zoom) + cam_data.position.y);
      
      std::string name = StringUtils::GetNameFromFilePath(path);
      Entity e = scene_->CreateEntity(name);
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
      
      auto& qc = e.AddComponent<QuadComponent>();
      qc.sprite.texture.push_back(Renderer::GetTexture(path));
    }
  }
  
  void GameData::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    scene_ = scene;
    viewport_ = viewport;
  }

  void GameData::MoveEntities(Direction direction, const std::unordered_map<entt::entity, Entity*>& selected_entities) {
    for (auto& [entt, entity] : selected_entities) {
      if(!entity) continue;
      
      auto& tc = entity->GetComponent<TransformComponent>();
      switch (direction) {
        case Down:      tc.AddPosition(Y, -1.0f);     break;
        case Up:        tc.AddPosition(Y, 1.0f);       break;
        case Right:     tc.AddPosition(X, 1.0f);       break;
        case Left:      tc.AddPosition(X, -1.0f);     break;
        default: break;
      }
    }
  }

} // namespace kreator
