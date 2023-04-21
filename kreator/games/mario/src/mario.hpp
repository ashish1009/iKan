//
//  mario.hpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#pragma once

#include "game_data.hpp"

namespace mario {
  
  using namespace kreator;
  using namespace ikan;
  
  struct TextData {
    glm::vec2 size = { 0.6f, 0.6f };
    const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    float row_pos[2];
    float col_pos[4];
    
    void SetViewport(float width, float height) {      
      static const glm::vec2 size_ref = { 0.6f, 0.6f };
      size = size_ref * width / 1600.0f;
      
      row_pos[0] = (float)height - 50;
      row_pos[1] = (float)height - 80;
      
      col_pos[0] = 50.0f;
      col_pos[1] = ((float)width * (1.0f / 4.0f)) + 50.0f;
      col_pos[2] = ((float)width * (2.0f / 4.0f)) + 50.0f;
      col_pos[3] = ((float)width * (3.0f / 4.0f)) + 50.0f;
    }
    
    void Render(const std::string& title, uint32_t row, uint32_t col) {
      TextRenderer::RenderFixedViewText(title, { col_pos[col], row_pos[row], 0.3f }, size, color);
    }
  };
  
  class Mario : public GameData {
  public:
    void Init(const std::shared_ptr<Scene> scene) override;
    void Update(Timestep ts) override;
    void SetViewportSize(uint32_t width, uint32_t height) override;

    std::string GameName() const override { return "IKAN Mario"; }
    glm::vec4 GetBgColor() const override { return {0.2, 0.4, 0.6, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/games/mario/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "Mario_scene.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    Font RegularFontData() const override { return {DM::ClientAsset("fonts/mario.ttf"), 14}; };
    Font BoldFontData() const override { return {DM::ClientAsset("fonts/mario.ttf"), 14}; };
    std::vector<std::filesystem::path> FavDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
      };
    };
    
  private:
    std::shared_ptr<Scene> scene_;
    TextData text_data_;

    const uint32_t MaxTime = 100;
    
    uint32_t world_ = 1;
    uint32_t level_ = 1;
    uint32_t time_left_ = 0;
    
    float timer_ = 0;
  };
  
} // namespace mario
