//
//  player.hpp
//  kreator
//
//  Created by Ashish . on 03/05/23.
//

#pragma once

#include "setting.hpp"
namespace chess {
  
  using namespace ikan;
  
  class Player {
  public:
    Player(const std::string& name, Color color) : name_(name), color_(color) { }
    
    const std::string& GetName() const { return name_; }
    Color GetColor() const { return color_; }
    
    /// This function Render GUI Widget for player data setting
    void RenderGui();
    
  private:
    std::string name_;
    Color color_;
  };
  
} // namespace chess
