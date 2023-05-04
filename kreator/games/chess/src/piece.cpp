//
//  piece.cpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#include "piece.hpp"
#include "block.hpp"

namespace chess {
  
#define MaxColVal MaxCols - 1
  
  PieceRef Piece::Create(PieceType piece, Color color, const Position& position) {
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
  /// - Parameters:
  ///   - result: result reference
  ///   - start_pos: Current Position of Piece
  ///   - end_col: end column position. Ex. Pawn have just 1 step and Rook have all limit
  static void AddUpPossibleMove(std::vector<Position>& result, const Position& start_pos, int32_t end_col) {
    int32_t start_col = start_pos.col;
    int32_t row = start_pos.row;
    
    // If Current Position is already at top then return
    if (start_col >= MaxColVal) return;
    
    // Iterate all the positions to get possible position
    for (int32_t col = start_col; col < start_col + end_col; col++) {
      // If any column position is out ot Board then return
      if (col >= MaxColVal) return;
      
      BlockRef block = BlockManager::GetBlock(row, col);
      if (block->IsEmpty()) {
        
      }
      else {
        
      }
    }
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
