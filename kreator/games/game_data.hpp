//
//  game_data.hpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace kreator {
  
  using namespace ikan;
  
  enum class GameType {
    Editor, Mario, Chess, AngryBird
  };
  
  /// Game Data Interface
  class GameData {
  public:
    virtual ~GameData() = default;
    
    /// This function load the prefab on Droping the item
    /// - Parameter path: file path of prefab
    /// - Note: Not Interface
    virtual void LoadPrefab(const std::string& path);
    
    /// Initilize your game data here. This function triggers when scene in Open, Play or Edit in game
    /// - Note: Whenever Renderer Layer change the Scene pointer this function calls
    /// - Parameters:
    ///   - scene: This is the reference of created scene
    ///   - viewport: Viewport Data const reference
    virtual void Init(const std::shared_ptr<Scene> scene, Viewport* viewport) = 0;
    /// This funcion calls each frame in renderer Pass. Update or debug render your game.
    /// - Parameter ts: tiem step of frames
    virtual void Update(Timestep ts) = 0;
    /// This function update the viewport width of your game
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
    
    /// This function renders the Gui for Game
    virtual void RenderGui() {}
    
    /// This function returns name of your game
    virtual std::string GameName() const = 0;
    /// This function return your viewport background color
    virtual glm::vec4 GetBgColor() const = 0;
    /// This function returns the Regular Font information
    virtual Font RegularFontData() const = 0;
    /// This function returns the Bold Font information
    virtual Font BoldFontData() const = 0;
    /// This function returns the paths of directory you want in content browser panel as home
    virtual std::string CbpRootDir() const = 0;
    /// This function returns the paths of directory you want in content browser panel as favourite
    virtual std::vector<std::filesystem::path> FavDirecotries() const = 0;
    /// This function returns the paths of directory you want in content browser panel as assets
    virtual std::vector<std::filesystem::path> AssetDirecotries() const = 0;
    /// This function returns the path of folder where cliet scenes are saved
    virtual std::string GetScenePath() const = 0;
    /// This function returns the path of scene you want to open in begining
    virtual std::string SavedScene() const = 0;
    
    /// This function set the flag as playing
    /// - Parameter playing_flag: play flag
    virtual void SetPlaying(bool playing_flag) { is_playing_ = playing_flag; }
    /// this function returns is game playing
    virtual bool IsPlaying() { return is_playing_; }
    
  protected:
    bool is_playing_ = false;
    std::shared_ptr<Scene> scene_;
    Viewport* viewport_;
  };
  
  /// This function is the defination of game data. Implementaiton should be at client
  std::unique_ptr<GameData> CreateGameData(GameType game_type);
  
} // namespace kreator
