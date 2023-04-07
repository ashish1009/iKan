//
//  renderer_stats.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "renderer_stats.hpp"
#include "imgui/imgui_api.hpp"
#include "editor/property_grid.hpp"

namespace ikan {
  
  void RendererStatistics::Renderer2DStats::RenderGui(bool* is_open) {
    CHECK_WIDGET_FLAG(is_open);
    
    ImGui::Begin("Renderer 2D Stats", is_open);
    ImGui::SetNextWindowContentSize(ImVec2(360, 0.0f));
    ImGui::BeginChild("##RendererStats2D", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::Columns(6);
    
    ImGui::SetColumnWidth(0, 60);
    ImGui::Text("%d", max_quads);
    PropertyGrid::HoveredMsg("Max Quads");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(1, 60);
    ImGui::Text("%d", max_circles);
    PropertyGrid::HoveredMsg("Max Circles");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(2, 60);
    ImGui::Text("%d", max_lines);
    PropertyGrid::HoveredMsg("Max Lines");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(3, 60);
    ImGui::Text("%d", quads);
    PropertyGrid::HoveredMsg("Num Quads Rendered");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(4, 60);
    ImGui::Text("%d", circles);
    PropertyGrid::HoveredMsg("Num Circles Rendered");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(5, 60);
    ImGui::Text("%d", lines);
    PropertyGrid::HoveredMsg("Num Lines Rendered");
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::End();
  }
  
  void RendererStatistics::RenderGui(bool* is_open) {
    CHECK_WIDGET_FLAG(is_open);

    static std::string hovered_message = "";

    ImGui::Begin("Renderer Stats", is_open);
    ImGui::SetNextWindowContentSize(ImVec2(520.0f, 0.0f));
    ImGui::BeginChild("##RendererStats", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Columns(6);

    ImGui::SetColumnWidth(0, 60);
    ImGui::Text("%d", draw_calls);
    PropertyGrid::HoveredMsg("Draw Calls");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(1, 80);
    ImGui::Text("%d", vertex_count);
    PropertyGrid::HoveredMsg("Vertex Counts");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(2, 80);
    ImGui::Text("%d", index_count);
    PropertyGrid::HoveredMsg("Index Counts");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(3, 100);
    ImGui::Text("%.3f KB", (float)vertex_buffer_size / (float)1000.0f);
    hovered_message  = "Vertex Buffer Size (Not counting buffer used to render Text) \n";
    hovered_message += "MB    : " + std::to_string((float)vertex_buffer_size / (float)1000000.0f) + "\n";
    hovered_message += "KB    : " + std::to_string((float)vertex_buffer_size / (float)1000.0f) + "\n";
    hovered_message += "Bytes : " + std::to_string(vertex_buffer_size);
    PropertyGrid::HoveredMsg(hovered_message.c_str());
    ImGui::NextColumn();

    ImGui::SetColumnWidth(4, 100);
    ImGui::Text("%.3f KB", (float)index_buffer_size / (float)1000.0f);
    hovered_message  = "Index Buffer Size \n";
    hovered_message += "MB    : " + std::to_string((float)index_buffer_size / (float)1000000.0f) + "\n";
    hovered_message += "KB    : " + std::to_string((float)index_buffer_size / (float)1000.0f) + "\n";
    hovered_message += "Bytes : " + std::to_string(index_buffer_size);
    PropertyGrid::HoveredMsg(hovered_message.c_str());
    ImGui::NextColumn();

    ImGui::SetColumnWidth(5, 100);
    ImGui::Text("%.3f KB", (float)texture_buffer_size / (float)1000.0f);
    hovered_message  = "Texure Data Size \n";
    hovered_message += "MB    : " + std::to_string((float)texture_buffer_size / (float)1000000.0f) + "\n";
    hovered_message += "KB    : " + std::to_string((float)texture_buffer_size / (float)1000.0f) + "\n";
    hovered_message += "Bytes : " + std::to_string(texture_buffer_size);
    PropertyGrid::HoveredMsg(hovered_message.c_str());

    ImGui::NextColumn();
    
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::End();
  }
  
  void RendererStatistics::ResetEachFrame() {
    draw_calls = 0;
    index_count = 0;
    vertex_count = 0;
    
    stats_2d_.quads = 0;
    stats_2d_.circles = 0;
    stats_2d_.lines = 0;
  }
  
  void RendererStatistics::ResetAll() {
    ResetEachFrame();
    
    vertex_buffer_size = 0;
    index_buffer_size = 0;
    texture_buffer_size = 0;

    stats_2d_.max_quads = 0;
    stats_2d_.max_circles = 0;
    stats_2d_.max_lines = 0;
  }
  
  RendererStatistics& RendererStatistics::Get() {
    static RendererStatistics instance;
    return instance;
  }
  
} // namespace ikan
