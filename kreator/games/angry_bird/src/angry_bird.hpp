//
//  angry_bird.hpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#pragma once

#include "game_data.hpp"

namespace angry_bird {
  
  using namespace kreator;
  using namespace ikan;
  
  class AngryBird : public GameData {
  public:
    void Init(const std::shared_ptr<Scene> scene) override {};
    void Update(Timestep ts) override {}
    void SetViewportSize(uint32_t width, uint32_t height) override {}

    std::string GameName() const override { return "IKAN Angry Bird"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/games/angry_bird/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "AngryBird.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    std::vector<std::filesystem::path> FavDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
      };
    };
    Font RegularFontData() const override {
      return {DM::ClientAsset("fonts/Roboto/Regular.ttf"), 14};
    };
    Font BoldFontData() const override {
      return {DM::ClientAsset("fonts/Roboto/Bold.ttf"), 14};
    };
  };
  
} // namespace angry_bird
