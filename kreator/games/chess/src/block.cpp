//
//  block.cpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#include "block.hpp"

namespace chess {
  
  Block::Block() {
    CHESS_LOG("Creating Block Instance");
  }
  
  Block::~Block() {
    CHESS_LOG("Destroying Block Instance");
  }
  
  void Block::SetData(Color color, PieceType piece) {
    IK_INFO("", "{0}, {1}", GetColorString(color), GetPieceString(piece));
  }

} // namespace chess
