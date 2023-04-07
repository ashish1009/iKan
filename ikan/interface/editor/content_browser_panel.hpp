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
    /// This constructor creates content Browser Panel instance with root path
    /// - Parameters:
    ///   - root_path: root path of Content browser panel
    ///   - favourite_paths: favourit paths in side bar
    ContentBrowserPanel(const std::string& root_path, const std::vector<std::filesystem::path>& favourite_paths = {});
    /// This destructor destroy Content browser panel instance
    ~ContentBrowserPanel();

    /// This function update the root path of CBP
    /// - Parameter root_path: root path
    void SetRootPath(const std::string& root_path);
    /// This funcfion add the favourit paths to the pannel
    /// - Parameter favourite_paths: paths
    void AddFavouritPaths(const std::vector<std::filesystem::path>& favourite_paths);

    /// This function returns the Current directory
    const std::filesystem::path& GetCurrentDir() const { return current_directory_; }
    /// This function returns the Root directory
    const std::filesystem::path& GetRootDir() const { return root_path_; }
    
    /// This function renders Imgui for Contenct browser panel
    /// - Parameter is_open: flag to show or hide widget
    void RenderGui(bool* is_open = nullptr);

  private:
    std::filesystem::path root_path_, current_directory_;
    std::vector<std::filesystem::path> favourite_paths_;
  };
  
} // namespace ikan
