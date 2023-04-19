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
  
  class Mario : public GameData {
  public:
    void Init(const std::shared_ptr<Scene> scene) override {};
    
    std::string GameName() const override { return "IKAN Mario"; }
    glm::vec4 GetBgColor() const override { return {0.2, 0.4, 0.6, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/mario/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "Mario_scene.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    std::vector<std::filesystem::path> FavDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
      };
    };
    Font RegularFontData() const override {
      return {DM::ClientAsset("fonts/mario.ttf"), 14};
    };
    Font BoldFontData() const override {
      return {DM::ClientAsset("fonts/mario.ttf"), 14};
    };
  };
  
} // namespace editor
