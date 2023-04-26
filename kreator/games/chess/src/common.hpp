//
//  common.hpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#pragma once

#define CHESS_LOG(...) IK_TRACE("Chess", __VA_ARGS__);

namespace chess {
  
  static constexpr uint16_t MaxRows = 8;
  static constexpr uint16_t MaxCols = 8;
  
  static constexpr uint16_t BlockSize = 1;
  
} // namespace chess
