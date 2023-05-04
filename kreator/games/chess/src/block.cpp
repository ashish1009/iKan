//
//  block.cpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#include "block.hpp"

namespace chess {
  
  Block::Block(const Position& position) : position_(position) {
    CHESS_LOG("Creating the Block oat possition {0}, {1}", position_.row, position_.col);
  }
  
  Block::~Block() {
    CHESS_LOG("Destroying the Block of possition {0}, {1}", position_.row, position_.col);
  }
  
  void Block::SetData(Color color, PieceType piece_type, Entity* entity) {
    piece_ = Piece::Create(piece_type, color, position_, entity);
  }
  
  std::array<std::array<BlockRef, MaxCols>, MaxRows> BlockManager::blocks_;
  
  BlockRef BlockManager::GetBlock(int32_t row, int32_t col) {
    if (row < 0 or col < 0 or row >= MaxRows or col >= MaxCols) return nullptr;
    return blocks_.at(row).at(col);
  }
  
  void BlockManager::Clear() {
    for (auto& row_block : blocks_) {
      for (auto& block : row_block) {
        block.reset();
      }
    }
  }
  
} // namespace chess
