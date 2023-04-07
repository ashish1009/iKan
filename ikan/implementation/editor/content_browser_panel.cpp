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
    IK_CORE_TRACE(LogModule::ContentBrowserPanel, "Creating Content Browser Panel ... ");
    IK_CORE_TRACE(LogModule::ContentBrowserPanel, "  Root Path {0}", root_path_.string());
  }
  
  ContentBrowserPanel::~ContentBrowserPanel() {
    IK_CORE_WARN(LogModule::ContentBrowserPanel, "Destroying Content Browser Panel !!! ");
    IK_CORE_WARN(LogModule::ContentBrowserPanel, "  Root Path {0}", root_path_.string());
  }
  
}// namespace ikan
