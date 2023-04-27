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
  public:
    King(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~King() = default;
  };

  class Queen : public Piece {
  public:
    Queen(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~Queen() = default;
  };

  class Rook : public Piece {
  public:
    Rook(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~Rook() = default;
  };

  class Bishop : public Piece {
  public:
    Bishop(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~Bishop() = default;
  };

  class Knight : public Piece {
  public:
    Knight(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~Knight() = default;
  };

  class Pawn : public Piece {
  public:
    Pawn(PieceType piece, Color color) {
      piece_ = piece;
      color_ = color;
    }
    ~Pawn() = default;
  };

} // namespace chess
