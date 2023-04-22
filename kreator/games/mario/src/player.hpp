//
//  player.hpp
//  kreator
//
//  Created by Ashish . on 23/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class PlayerAction {
    Invalid, Idle, Run, SwitchSide, Jump, Die, PowerUp
  };
  
  enum class PlayerState {
    Invalid, Small, Big, Fire
  };
  
} // namespace mario
