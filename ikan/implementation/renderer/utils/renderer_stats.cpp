//
//  renderer_stats.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "renderer_stats.hpp"

namespace ikan {
  
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
