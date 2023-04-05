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
    ImGui::PushID("Renderer 2D Stats");

    ImGui::PopID();
    ImGui::End();
  }
  
  void RendererStatistics::RenderGui(bool* is_open) {
    CHECK_WIDGET_FLAG(is_open);

    ImGui::Begin("Renderer Stats", is_open);
    ImGui::PushID("Renderer Stats");

    ImGui::PopID();
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
