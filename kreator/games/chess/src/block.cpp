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
  
  void Block::SetData(Color color, PieceType piece_type, Entity* entity) {
    piece_ = Piece::Create(piece_type, color, position_, entity);
  }
  
  std::array<std::array<BlockRef, MaxCols>, MaxRows> BlockManager::blocks_;
  
  BlockRef BlockManager::GetBlock(int32_t row, int32_t col) {
    if (row < 0 or col < 0 or row >= MaxRows or col >= MaxCols) return nullptr;
    return blocks_.at(row).at(col);
  }
  
} // namespace chess
