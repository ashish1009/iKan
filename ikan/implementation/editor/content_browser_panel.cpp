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
  
  static constexpr float round_factor_ = 1.0f;
  static constexpr glm::vec2 icon_size_ = {18.0f, 18.0f};
  
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

    TitleIcon();
    
    // Side Menue should be called before MainArea
    SideMenu();
    ImGui::SameLine();
    
    MainArea();

    ImGui::PopID(); // Content Browser
    ImGui::End(); // Content Browser
  }

  void ContentBrowserPanel::TitleIcon() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, round_factor_);
    ImGui::BeginChild("Title Area", ImVec2(ImGui::GetWindowContentRegionWidth(), 35.0f), true /* Border */, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Columns(3);
    ImGui::SetColumnWidth(0, 100);
    
    Back();             ImGui::SameLine();
    Forward();          ImGui::SameLine();
    Home();             ImGui::SameLine();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    Search();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    PathHistory();
    ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::PopStyleVar();
  }
  
  void ContentBrowserPanel::SideMenu() {
  }
  
  void ContentBrowserPanel::MainArea() {
  }
  
  void ContentBrowserPanel::Back() {
    static std::shared_ptr<Texture> back_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/back.png"));
    if (PropertyGrid::ImageButton("Back", back_texture->GetRendererID(), icon_size_)) {
    }
  }
  
  void ContentBrowserPanel::Forward() {
    static std::shared_ptr<Texture> forward_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/forward.png"));
    if (PropertyGrid::ImageButton("Forward", forward_texture->GetRendererID(), icon_size_)) {
    }
  }
  
  void ContentBrowserPanel::Home() {
    static std::shared_ptr<Texture> home_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/home.png"));
    if (PropertyGrid::ImageButton("home", home_texture->GetRendererID(), icon_size_)) {
    }
  }
  
  void ContentBrowserPanel::Search() {
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16);
    if (PropertyGrid::Search(search_filter_.InputBuf, "Search ... ", icon_size_)) {
      search_filter_.Build();
    }
  }
  
  void ContentBrowserPanel::PathHistory() {
  }

}// namespace ikan
