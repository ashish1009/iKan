//
//  content_browser_panel.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "content_browser_panel.hpp"
#include "editor/property_grid.hpp"
#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  ContentBrowserPanel::ContentBrowserPanel(const std::string& root_path, const std::vector<std::filesystem::path>& favourite_paths)
  : root_path_(root_path), current_directory_(root_path) {
    // Add Pinned Paths
    favourite_paths_.emplace_back(DM::GetWorkspaceBasePath() + "ikan/core_assets");
    for (const auto& path : favourite_paths)
      favourite_paths_.emplace_back(path);

    IK_CORE_TRACE(LogModule::ContentBrowserPanel, "Creating Content Browser Panel ... ");
    IK_CORE_TRACE(LogModule::ContentBrowserPanel, "  Root Path {0}", root_path_.string());
  }
  
  ContentBrowserPanel::~ContentBrowserPanel() {
    IK_CORE_WARN(LogModule::ContentBrowserPanel, "Destroying Content Browser Panel !!! ");
    IK_CORE_WARN(LogModule::ContentBrowserPanel, "  Root Path {0}", root_path_.string());
  }
  
  void ContentBrowserPanel::SetRootPath(const std::string& root_path) {
    root_path_ = root_path;
    current_directory_ = root_path;
  }
  
  void ContentBrowserPanel::AddFavouritPaths(const std::vector<std::filesystem::path>& favourite_paths) {
    for (const auto& path : favourite_paths)
      favourite_paths_.emplace_back(path);
    
  }
  void ContentBrowserPanel::RenderGui(bool* is_open) {
    CHECK_WIDGET_FLAG(is_open);
    
    ImGui::Begin("Content Browser", is_open);
    ImGui::PushID("Content Browser");

    ImGui::PopID(); // Content Browser
    ImGui::End(); // Content Browser
  }

}// namespace ikan
