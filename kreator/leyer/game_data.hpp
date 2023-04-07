//
//  game_data.hpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace kreator {
  
  using namespace ikan;
  
  enum class GameType {
    Editor
  };
  
  class GameData {
  public:
    virtual ~GameData() = default;
    
    /// This function returns name of your game
    virtual std::string GameName() const = 0;
    /// This function return your viewport background color
    virtual glm::vec4 GetBgColor() const = 0;
    /// This function returns the Regular Font information
    virtual Font RegularFontData() const = 0;
    /// This function returns the Bold Font information
    virtual Font BoldFontData() const = 0;
  };
  
  class GameDataImpl : public GameData {
  public:
    std::string GameName() const override { return "Kreator Editor"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    Font RegularFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14};
    };
    Font BoldFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14};
    };
  };
  
  /// This function is the defination of game data. Implementaiton should be at client
  std::unique_ptr<GameData> CreateGameData(GameType game_type);
  
} // namespace kreator
