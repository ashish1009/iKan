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
      case PieceType::King:     return std::make_shared<King>(piece, color);
      case PieceType::Queen:    return std::make_shared<Queen>(piece, color);
      case PieceType::Rook:     return std::make_shared<Rook>(piece, color);
      case PieceType::Bishop:   return std::make_shared<Bishop>(piece, color);
      case PieceType::Knight:   return std::make_shared<Knight>(piece, color);
      case PieceType::Pawn:     return std::make_shared<Pawn>(piece, color);
      default: IK_ASSERT(false);
    }
  }
  
} // namespace chess
