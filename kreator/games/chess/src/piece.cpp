//
//  piece.cpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#include "piece.hpp"

namespace chess {
  
  std::shared_ptr<Piece> Piece::Create(PieceType piece, Color color, const Position& position) {
    switch (piece) {
      case PieceType::King:     return std::make_shared<King>(piece, color, position);
      case PieceType::Queen:    return std::make_shared<Queen>(piece, color, position);
      case PieceType::Rook:     return std::make_shared<Rook>(piece, color, position);
      case PieceType::Bishop:   return std::make_shared<Bishop>(piece, color, position);
      case PieceType::Knight:   return std::make_shared<Knight>(piece, color, position);
      case PieceType::Pawn:     return std::make_shared<Pawn>(piece, color, position);
      default: IK_ASSERT(false);
    }
  }
  
  /// This function add the possible moves in Straight Up Direction
  /// - Note: Adds only if possible
  /// - Parameter result: result reference
  static void AddUpPossibleMove(std::vector<Position>& result) {
    
  }
  
  std::vector<Position> King::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  std::vector<Position> Queen::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  std::vector<Position> Rook::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  std::vector<Position> Bishop::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  std::vector<Position> Knight::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  std::vector<Position> Pawn::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

} // namespace chess
