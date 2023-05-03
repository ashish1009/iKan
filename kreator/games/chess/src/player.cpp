//
//  player.cpp
//  kreator
//
//  Created by Ashish . on 03/05/23.
//

#include "player.hpp"

namespace chess {

  void Player::RenderGui() {
    ImGui::Begin(GetColorString(color_).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
    PropertyGrid::TextBox(name_);
    PropertyGrid::HoveredMsg("Change Player Name from Here...");
    ImGui::End();
  }
  
} // namespace chess
