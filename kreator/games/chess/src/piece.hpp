//
//  piece.hpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#pragma once

#include "setting.hpp"

#define CONSTRUCT_DESTRUCT(PieceClass) \
  PieceClass(PieceType piece, Color color, const Position& position) { \
position_ = position; \
    piece_ = piece; \
    color_ = color; \
  } \
  ~PieceClass() = default;

#define DEFINE_VIRTUAL_APIS() \
  std::vector<Position> GetPossibleMoves() override;


namespace chess {
  
#define PieceRef std::shared_ptr<Piece>

  class Piece {
  public:
    ~Piece() = default;
    
    /// This function returns the positions of possible moves of the selected piece
    virtual std::vector<Position> GetPossibleMoves() = 0;
    
    /// This function creates the instancec of piece based on type and color
    /// - Parameters:
    ///   - piece: piece type
    ///   - color: color of piece
    static PieceRef Create(PieceType piece, Color color, const Position& position);
    
    /// This function returns the Piece type
    PieceType GetPiece() const { return piece_; }
    /// This function returns the color of piece
    Color GetColor() const { return color_; }
    
  protected:
    PieceType piece_;
    Color color_;
    Position position_;
  };
  
  class King : public Piece {
  public:
    CONSTRUCT_DESTRUCT(King);
    DEFINE_VIRTUAL_APIS();
  };

  class Queen : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Queen);
    DEFINE_VIRTUAL_APIS();
  };

  class Rook : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Rook);
    DEFINE_VIRTUAL_APIS();
  };

  class Bishop : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Bishop);
    DEFINE_VIRTUAL_APIS();
  };

  class Knight : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Knight);
    DEFINE_VIRTUAL_APIS();
  };

  class Pawn : public Piece {
  public:
    CONSTRUCT_DESTRUCT(Pawn);
    DEFINE_VIRTUAL_APIS();
  };

} // namespace chess
