//
//  player.hpp
//  kreator
//
//  Created by Ashish . on 03/05/23.
//

#pragma once

#include "setting.hpp"
namespace chess {
  
  class Player {
  public:
    const std::string& GetName() const { return name_; }
    Color GetColor() const { return color; }
    
  private:
    std::string name_ = "Player";
    Color color;
  };
  
} // namespace chess
