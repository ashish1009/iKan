//
//  chess.hpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#pragma once

#include "game_data.hpp"
#include "setting.hpp"
#include "block.hpp"
#include "player.hpp"

namespace chess {
  
  using namespace kreator;
  using namespace ikan;
  
  class Chess : public GameData {
  public:
    Chess();
    ~Chess();

    void LoadPrefab(const std::string& path) override;

    void Init(const std::shared_ptr<Scene> scene, Viewport* viewport) override;
    void Update(Timestep ts) override;
    void EventHandler(Event& event) override;
    void SetViewportSize(uint32_t width, uint32_t height) override;
    void RenderGui() override;

    void SetPlaying(bool playing_flag) override;

    // Game Data API
    std::string GameName() const override { return "IKAN Chess"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::WorkspacePath("/kreator/games/chess/scenes/"); }
    std::string SavedScene() const override { return GetScenePath() + "Chess.ikanScene"; };
    std::string CbpRootDir() const override { return GetScenePath(); };
    Font RegularFontData() const override { return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14}; };
    Font BoldFontData() const override { return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14}; };
    std::vector<std::filesystem::path> FavDirecotries() const override { return { DM::WorkspacePath("/kreator") }; };
    std::vector<std::filesystem::path> AssetDirecotries() const override {
      return {
        GetScenePath(),
        DM::ClientAsset("textures"),
        DM::ClientAsset("prefabs"),
      };
    };
    
  private:
    /// This function dispatched in event dispatcher and trigger when mouse move event evoked
    /// - Parameter mouse_move_event: Mouse Move event instacnce
    bool MouseMoved(MouseMovedEvent& mouse_move_event);
    /// This function dispatched in event dispatcher and trigger when mouse button press event evoked
    /// - Parameter mouse_click_event: Mouse button press event instacnce
    bool MouseClicked(MouseButtonPressedEvent& mouse_click_event);
    /// This function returns the position of Block if mouse hovered
    glm::vec2 GetBlockPosition();
    /// This function renders the chess Block Grids
    void RenderChessGrids();
    /// This function renders the background and border
    void RenderBackgroundAndBorder();
    /// This function renders the text of game
    void RenderText();
    /// This function renders the player information
    void RenderPlayerInfo();
    /// This function highlight the hovered block
    void HighlightHoveredBlock();
    /// This function create
    /// - Note: Block Name should be "block"
    /// - Note: Piece Name should be {color_<piece> where piece : {pawn, king, queen, knight, bishop, rook}}
    void CreateBlocks();
    
    // Member data
    uint32_t viewport_width_ = 0, viewport_height_ = 0;
    
    glm::vec2 mouse_pos_;
    glm::vec2 init_cam_pos_;
    
    Block* hovered_block_ = nullptr;
    
    std::array<std::shared_ptr<Player>, MaxPlayer> players_;
  };
  
} // namespace chess
