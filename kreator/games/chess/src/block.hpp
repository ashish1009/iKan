//
//  block.hpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#pragma once

#include "setting.hpp"
#include "piece.hpp"

namespace chess {
  
#define BlockRef std::shared_ptr<Block>
  
  class Block {
  public:
    /// This Constructor creates the Block Data
    /// - Parameters:
    ///   - row_: Row index of block
    ///   - col_: column index of block
    Block(const Position& position);
    /// This Constructor destroy the Block Data
    ~Block();
    
    /// This function set the Block Data if present
    /// - Parameters:
    ///   - color: color of piece
    ///   - piece: type of piece
    void SetData(Color color, PieceType piece);
    
    /// This function returns the row index of block
    int32_t GetRow() const { return position_.row; }
    /// This function returns the column index of block
    int32_t GetCol() const { return position_.col; }
    /// This function returns the Piece on this block
    std::shared_ptr<Piece> GetPiece() { return piece_; }
    
  private:
    std::shared_ptr<Piece> piece_;
    Position position_;
  };
  
  class Chess;
  class BlockManager {
  private:
    static std::array<std::array<BlockRef, MaxCols>, MaxRows> blocks_;
    friend class Chess;
  };
    
} // namespace chess
