//
//  piece.cpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#include "piece.hpp"

namespace chess {
  
  std::shared_ptr<Piece> Piece::Create(PieceType piece, Color color) {
    switch (piece) {
      case PieceType::King:     return std::make_shared<King>();
      case PieceType::Queen:    return std::make_shared<Queen>();
      case PieceType::Rook:     return std::make_shared<Rook>();
      case PieceType::Bishop:   return std::make_shared<Bishop>();
      case PieceType::Knight:   return std::make_shared<Knight>();
      case PieceType::Pawn:     return std::make_shared<Pawn>();
      default: IK_ASSERT(false);
    }
  }
  
} // namespace chess
