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
#define MaxRowVal MaxRows - 1

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
  ///   - end_row: end row position. Ex. Pawn have just 1 step and Rook have all limit
  ///   - selected_piece_color: selected piece color
  ///   - is_pawn: is current piece is pawn
  static void AddUpPossibleMove(std::vector<Position>& result, const Position& start_pos, int32_t end_row, Color selected_piece_color, bool is_pawn) {
    const int32_t col = start_pos.col;
    const int32_t start_row = start_pos.row;
    
    // If Current Position is already at top then return
    if (start_row >= MaxRowVal) return;
    
    // Iterate all the positions to get possible position
    int32_t row = start_row;
    while (row < end_row) {
      // Move iterater as we do not want to process current position
      row++;

      BlockRef block = BlockManager::GetBlock(row, col);
      if (block->IsEmpty()) {
        result.push_back({row, col});
      }
      else {
        if (!is_pawn) {
          PieceRef piece = block->GetPiece();
          // If Block is not empty but of opponent color then still this is possible
          if (piece->GetColor() != selected_piece_color) {
            result.push_back({row, col});
          }
          else {
            // Do Nothing
          }
        }
        else {
          // Pawns do not Cut straight fight
        }
      }
      
      // If current column is at boundary of board then break
      if (col >= MaxColVal) break;
    }
  }
  
  const std::vector<Position>& King::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  const std::vector<Position>& Queen::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  const std::vector<Position>& Rook::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  const std::vector<Position>& Bishop::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  const std::vector<Position>& Knight::GetPossibleMoves() {
    std::vector<Position> result;
    return result;
  }

  const std::vector<Position>& Pawn::GetPossibleMoves() {
    possible_moves_.clear();
    AddUpPossibleMove(possible_moves_, position_, position_.col + 1, color_, true);
    return possible_moves_;
  }

} // namespace chess
