//
//  commando.hpp
//  kreator
//
//  Created by Ashish . on 08/05/23.
//

#pragma once

#include "game_data.hpp"

namespace commando {
  
  using namespace ikan;
  using namespace kreator;
  
  class Commando : public GameData {
  public:
    Commando();
    ~Commando();
    
    void AddQuadFromTexture(const std::string& path) override;
    void MoveEntities(Direction direction, const std::unordered_map<entt::entity, Entity*>& selected_entities) override;

    void Init(const std::shared_ptr<Scene> scene, Viewport* viewport) override;
    void Update(Timestep ts) override;
    void SetViewportSize(uint32_t width, uint32_t height) override {}
    
    std::string GameName() const override { return "IKAN Commando"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/games/commando/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "Commando.ikanScene"; };
    std::string CbpRootDir() const override { return DM::ClientAsset("textures/Isometric"); };
    Font RegularFontData() const override { return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14}; };
    Font BoldFontData() const override { return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14}; };
    std::vector<std::filesystem::path> FavDirecotries() const override { return { DM::WorkspacePath("/kreator") }; };
    std::vector<std::filesystem::path> AssetDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
        DM::ClientAsset("textures/Isometric"),
        DM::ClientAsset("textures/Characters"),
        DM::ClientAsset("textures/Angle"),
        DM::ClientAsset("prefabs"),
      };
    };
  };
  
} // namespace commando
