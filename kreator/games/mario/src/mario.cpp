//
//  mario.cpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#include "mario.hpp"
#include "sprite_manager.hpp"

namespace mario {
  
  static const std::string MarioLogTag = "Mario";
  
  Mario::Mario() {
    IK_INFO(MarioLogTag, "Creating Mario Game Data ... ");
    Batch2DRenderer::AddQuadData(2000);
    SpriteManager::Init();
  }
  
  Mario::~Mario() {
    IK_WARN(MarioLogTag, "Destroying Mario Game Data ... ");
    SpriteManager::Shutdown();
  }
  
  void Mario::Init(const std::shared_ptr<Scene> scene) {
    scene_ = scene;
    
    timer_ = 0;
    time_left_ = MaxTime;
    
    SearchOrCreatePlayer();
  }
  
  void Mario::Update(Timestep ts) {
    if (is_playing_) {
      // Timer
      timer_ += ts;
      time_left_ = MaxTime - (uint32_t)timer_;
    }

    // Score and All text
    TextRenderer::BeginBatch(FixedCamera::projection);
    
    text_data_.Render("MARIO", 0, 0);
    text_data_.Render(std::to_string(0), 1, 0);
    
    Batch2DRenderer::BeginBatch(FixedCamera::projection);
    Batch2DRenderer::DrawQuad(Math::GetTransformMatrix({text_data_.col_pos[1] - 10, text_data_.row_pos[1] + 8, 0},
                                                       {0, 0, 0}, {20, 20, 1}), {1, 1, 1, 1});
    Batch2DRenderer::EndBatch();

    text_data_.Render(" x " + std::to_string(0), 1, 1);
    
    text_data_.Render("WORLD", 0, 2);
    text_data_.Render(std::to_string(world_) + " - " + std::to_string(level_), 1, 2);
    
    text_data_.Render("TIME", 0, 3);
    text_data_.Render(std::to_string(time_left_), 1, 3);
    
    TextRenderer::EndBatch();
  }
  
  void Mario::SearchOrCreatePlayer() {
    const std::string player_name = "Mario Player";
    bool found_player = false;
    Entity player_entity;

    // Search for Player in current scene
    auto tag_view = scene_->GetEntitesWith<TagComponent>();
    for (auto entity : tag_view) {
      const auto& player_tag = tag_view.get<TagComponent>(entity).tag;
      // - Note: This has to be the payer Name in the game
      if (player_tag == player_name) {
        found_player = true;
        player_entity = Entity(entity, scene_.get());
        break;
      }
    }
    
    if (!found_player) {
      player_entity = scene_->CreateEntity(player_name);
    }
  }
  
  void Mario::SetViewportSize(uint32_t width, uint32_t height) {
    text_data_.SetViewport(width, height);
  }

} // namespace mario
