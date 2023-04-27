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
  
  std::string GetPieceString(PieceType piece) {
    switch (piece) {
      case PieceType::King:     return "King";
      case PieceType::Queen:    return "Queen";
      case PieceType::Rook:     return "Rook";
      case PieceType::Bishop:   return "Bishop";
      case PieceType::Knight:   return "Knight";
      case PieceType::Pawn:     return "Pawn";
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

  PieceType GetPieceFromTag(const std::string& piece_tag) {
    auto underscore = piece_tag.find_last_of("_");
    IK_ASSERT(underscore != std::string::npos);
    std::string piece = piece_tag.substr(underscore + 1, piece_tag.size());
    
    if (piece == "king") return     PieceType::King;
    if (piece == "queen") return    PieceType::Queen;
    if (piece == "rook") return     PieceType::Rook;
    if (piece == "bishop") return   PieceType::Bishop;
    if (piece == "knight") return   PieceType::Knight;
    if (piece == "pawn") return     PieceType::Pawn;

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
