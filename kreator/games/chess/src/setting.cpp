//
//  setting.cpp
//  kreator
//
//  Created by Ashish . on 27/04/23.
//

#include "setting.hpp"

namespace chess {
    
  std::string GetColorString(Color color) {
    switch (color) {
      case Color::Black: return "Black";
      case Color::White: return "White";
      default: IK_ASSERT(false);
    }
  }
  
  std::string GetPieceString(Piece piece) {
    switch (piece) {
      case Piece::King:     return "King";
      case Piece::Queen:    return "Queen";
      case Piece::Rook:     return "Rook";
      case Piece::Bishop:   return "Bishop";
      case Piece::Knight:   return "Knight";
      case Piece::Pawn:     return "Pawn";
      default: IK_ASSERT(false);
    }
  }
  
  Color GetColorFromTag(const std::string& piece_tag) {
    auto underscore = piece_tag.find_last_of("_");
    IK_ASSERT(underscore != std::string::npos);
    std::string color = piece_tag.substr(0, underscore);
    
    if (color == "black") return Color::Black;
    if (color == "white") return Color::White;
    
    IK_ASSERT(false);
  }

  Piece GetPieceFromTag(const std::string& piece_tag) {
    auto underscore = piece_tag.find_last_of("_");
    IK_ASSERT(underscore != std::string::npos);
    std::string piece = piece_tag.substr(underscore + 1, piece_tag.size());
    
    if (piece == "king") return     Piece::King;
    if (piece == "queen") return    Piece::Queen;
    if (piece == "rook") return     Piece::Rook;
    if (piece == "bishop") return   Piece::Bishop;
    if (piece == "knight") return   Piece::Knight;
    if (piece == "pawn") return     Piece::Pawn;

    IK_ASSERT(false);
  }

  bool IsPieceTag(const std::string& piece_tag) {
    auto underscore = piece_tag.find_last_of("_");
    if (underscore == std::string::npos)
      return false;
    
    // Assuming tag contains <color_piece>
    return true;
  }
  
} // namespace mario
