//
//  chess.hpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#pragma once

#include "game_data.hpp"

namespace chess {
  
  using namespace kreator;
  using namespace ikan;
  
  class Chess : public GameData {
  public:
    Chess();
    ~Chess();
    
    void Init(const std::shared_ptr<Scene> scene) override;
    void Update(Timestep ts) override;
    void SetViewportSize(uint32_t width, uint32_t height) override;

    // Game Data API
    std::string GameName() const override { return "IKAN Chess"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/games/chess/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "Chess.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    Font RegularFontData() const override { return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14}; };
    Font BoldFontData() const override { return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14}; };
    std::vector<std::filesystem::path> FavDirecotries() const override { return { DM::WorkspacePath("/kreator") }; };
    std::vector<std::filesystem::path> AssetDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
        DM::ClientAsset("prefabs"),
      };
    };
    
  private:
    /// This function renders the chess Block Grids
    void RenderChessGrids();
    /// This function renders the background and border
    void RenderBackgroundAndBorder();
    /// This function renders the text of game
    void RenderText();
    
    std::shared_ptr<Scene> scene_;
    uint32_t viewport_width_ = 0, viewport_height_ = 0;
  };
  
} // namespace chess
