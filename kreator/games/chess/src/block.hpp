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
    ///   - entity: entity of piece
    void SetData(Color color, PieceType piece, Entity* entity);
    
    /// This function returns the row index of block
    int32_t GetRow() const { return position_.row; }
    /// This function returns the column index of block
    int32_t GetCol() const { return position_.col; }
    /// This function returns the Piece on this block
    PieceRef GetPiece() { return piece_; }
    
    /// This function checks the Block is empty or not
    bool IsEmpty() const { return piece_ == nullptr; }
    
  private:
    PieceRef piece_;
    Position position_;
  };
  
  class Chess;
  class BlockManager {
  public:
    /// This function returns the block pointer at row and column
    /// - Parameters:
    ///   - row: row numebr
    ///   - col: column number
    static BlockRef GetBlock(int32_t row, int32_t col);
    /// This funciton clear the blocks
    static void Clear();
    
  private:
    static std::array<std::array<BlockRef, MaxCols>, MaxRows> blocks_;
    friend class Chess;
  };
    
} // namespace chess
