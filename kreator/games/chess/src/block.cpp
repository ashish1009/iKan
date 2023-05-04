//
//  block.cpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#include "block.hpp"

namespace chess {
  
  Block::Block(const Position& position) : position_(position) {}
  
  Block::~Block() { }
  
  void Block::SetData(Color color, PieceType piece_type) {
    piece_ = Piece::Create(piece_type, color, position_);
  }
  
  std::array<std::array<BlockRef, MaxCols>, MaxRows> BlockManager::blocks_;
  
} // namespace chess
