//
//  setting.hpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#pragma once

#define CHESS_LOG(...) IK_TRACE("Chess", __VA_ARGS__);

namespace chess {
  
  static constexpr uint16_t MaxRows = 8;
  static constexpr uint16_t MaxCols = 8;
  
  // Should be same as Prefab/Piece Height
  static constexpr uint16_t BlockSize = 2;
  
  enum class Color {
    Black, White
  };
  
  enum class Piece {
    King, Queen, Rook, Bishop, Knight, Pawn
  };
  
  /// This function returns the color string
  /// - Parameter color: color enum
  std::string GetColorString(Color color);
  /// This function returns the color string
  /// - Parameter color: color enum
  std::string GetPieceString(Piece color);

  /// This function return the color type from entity tag
  /// - Parameter tag: entity Tag
  /// - Note: Tag should be in form of color_<piece type>
  /// - Note: color name should be <"black" or "white">
  Color GetColorFromTag(const std::string& piece_tag);
  /// This function return the piece type from entity tag
  /// - Parameter tag: entity Tag
  /// - Note: Tag should be in form of color_<piece type>
  /// - Note: piece name should be <"king queen rook bioshop knight pawn">
  Piece GetPieceFromTag(const std::string& piece_tag);

  /// This function return if the entity is piece
  /// - Parameter tag: entity Tag
  /// - Note: Tag should be in form of color_<piece type>
  bool IsPieceTag(const std::string& piece_tag);
  
} // namespace chess
