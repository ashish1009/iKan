//
//  piece.hpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#pragma once

#include "setting.hpp"

#define CONSTRUCT_DESTRUCT(PieceClass) \
PieceClass(PieceType piece, Color color) { \
piece_ = piece; \
color_ = color; \
} \
~PieceClass() = default; \


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
  public:
    CONSTRUCT_DESTRUCT(King);
  };

  class Queen : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Queen);
  };

  class Rook : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Rook);
  };

  class Bishop : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Bishop);
  };

  class Knight : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Knight);
  };

  class Pawn : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Pawn);
  };

} // namespace chess
