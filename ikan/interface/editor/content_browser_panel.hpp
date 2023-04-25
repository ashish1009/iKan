//
//  content_browser_panel.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

namespace ikan {
  
  /// This class renders imgui window to access contents of folders
  class ContentBrowserPanel {
  public:
    /// This function destory any created Data
    static void Shutdown();
    /// This function update the root path of CBP
    /// - Parameter root_path: root path
    static void SetRootPath(const std::string& root_path);
    /// This funcfion add the favourit paths to the pannel
    /// - Parameter favourite_paths: paths
    /// - Note: Paths should be absolute
    static void AddFavouritPaths(const std::vector<std::filesystem::path>& favourite_paths);
    /// This funcfion add the Asset paths to the pannel
    /// - Parameter asset_paths: paths
    /// - Note: Paths should be absolute
    static void AddAssetPaths(const std::vector<std::filesystem::path>& asset_paths);

    /// This function returns the Current directory
    static const std::filesystem::path& GetCurrentDir() { return current_directory_; }
    /// This function returns the Root directory
    static const std::filesystem::path& GetRootDir() { return root_path_; }
    
    /// This function clear all Favourite and assits paths
    static void ClearAllPaths();
    
    /// This function renders Imgui for Contenct browser panel
    /// - Parameter is_open: flag to show or hide widget
    static void RenderGui(bool* is_open = nullptr);

    MAKE_PURE_STATIC(ContentBrowserPanel);
    
  private:
    /// This function renders the title of Content browser pannel
    static void TitleIcon();
    /// This function renders the Main area of Content browser pannel
    static void MainArea();
    /// This function renders the Main area of Content browser pannel
    static void SideMenu();

    /// This function render back icon
    static void Back();
    /// This function render forward icon
    static void Forward();
    /// This function render home icon
    static void Home();
    
    /// This function render search Text box icon
    static void Search();
    /// This fucntion prints the path history
    static void PathHistory();
    
    inline static float side_child_width_ = 0.15f;
    
    inline static std::filesystem::path root_path_, current_directory_;
    inline static std::vector<std::filesystem::path> favourite_paths_;
    inline static std::vector<std::filesystem::path> asset_paths_;
    inline static ImGuiTextFilter search_filter_;

    inline static std::vector<std::filesystem::path> back_path_history_;
    inline static std::vector<std::filesystem::path> forward_path_history_;
    inline static std::vector<std::filesystem::path> path_hierarchy_;
  };
  
  using CBP = ContentBrowserPanel;
  
} // namespace ikan
