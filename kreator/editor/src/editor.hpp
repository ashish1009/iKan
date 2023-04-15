//
//  editor.hpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#pragma once

#include "game_data.hpp"

namespace editor {
  
  using namespace kreator;
  using namespace ikan;
  
  class Editor : public GameData {
  public:
    void Init(const std::shared_ptr<Scene> scene) override {};
    
    std::string GameName() const override { return "Kreator Editor"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/editor/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "/first_scene.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    std::vector<std::filesystem::path> FavDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
      };
    };
    Font RegularFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14};
    };
    Font BoldFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14};
    };
  };
  
} // namespace editor
