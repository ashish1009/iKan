//
//  block.cpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#include "block.hpp"

namespace chess {
  
  Block::Block(int32_t row, int32_t col) : row_(row), col_(col) {
    CHESS_LOG("Creating Block Instance");
  }
  
  Block::~Block() {
    CHESS_LOG("Destroying Block Instance");
  }
  
  void Block::SetData(Color color, PieceType piece_type) {
    piece_ = Piece::Create(piece_type, color);
  }
  
  std::array<std::array<BlockRef, MaxCols>, MaxRows> BlockManager::blocks_;
  
} // namespace chess
