//
//  game_data.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#include "game_data.hpp"
#include "editor.hpp"

namespace kreator {
  
  std::unique_ptr<GameData> CreateGameData(GameType game_type) {
    switch (game_type) {
      case GameType::Editor:
      default:
        return std::make_unique<editor::Editor>();
    }
  }
  
} // namespace kreator
