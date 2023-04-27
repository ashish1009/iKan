//
//  block.hpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#pragma once

#include "setting.hpp"

namespace chess {
  
#define BlockRef std::shared_ptr<Block>
  
  class Block {
  public:
    /// This Constructor creates the Block Data
    Block();
    /// This Constructor destroy the Block Data
    ~Block();
    
    /// This function set the Block Data if present
    /// - Parameters:
    ///   - color: color of piece
    ///   - piece: type of piece
    void SetData(Color color, PieceType piece);
    
  private:
    std::shared_ptr<PieceType> piece;
  };
    
} // namespace chess
