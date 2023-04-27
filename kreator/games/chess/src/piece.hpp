//
//  piece.hpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#pragma once

#include "setting.hpp"

namespace chess {
  
  class Piece {
  public:
    ~Piece() = default;
    
    /// This function creates the instancec of piece based on type and color
    /// - Parameters:
    ///   - piece: piece type
    ///   - color: color of piece
    static std::shared_ptr<Piece> Create(PieceType piece, Color color);
    
    /// This function returns the Piece type
    PieceType GetPiece() const { return piece_; }
    /// This function returns the color of piece
    Color GetColor() const { return color_; }
    
  protected:
    PieceType piece_;
    Color color_;
  };
  
  class King : public Piece {
  };

  class Queen : public Piece {
  };

  class Rook : public Piece {
  };

  class Bishop : public Piece {
  };

  class Knight : public Piece {
  };

  class Pawn : public Piece {
  };

} // namespace chess
