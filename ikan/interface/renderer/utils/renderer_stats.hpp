//
//  renderer_stats.h
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
    
  /// Stores the statistics of renderer
  struct RendererStatistics {
  private:
    struct Renderer2DStats {
      uint32_t max_quads = 0, max_circles = 0, max_lines = 0;
      uint32_t quads = 0, circles = 0, lines = 0;
      
      /// This function shows the stats in gui widget
      /// - Parameter is_open: flag to show or hide the widget
      void RenderGui(bool *is_open = nullptr);
    };
    
    RendererStatistics() = default;
    DELETE_COPY_MOVE_CONSTRUCTORS (RendererStatistics);

  public:
    uint32_t vertex_buffer_size = 0, index_buffer_size = 0, texture_buffer_size = 0;
    uint32_t vertex_count  = 0, index_count = 0;
    uint32_t draw_calls = 0;
    
    Renderer2DStats stats_2d_;
    
    /// This function resets only those stats that need to be reset each frame
    void ResetEachFrame();
    /// This function resets all stats
    void ResetAll();

    /// This function shows the stats in gui widget
    /// - Parameter is_open: flag to show or hide the widget
    void RenderGui(bool *is_open = nullptr);

    /// This function returns the static singleton instance of RendererStatistics
    static RendererStatistics& Get();
  };
  
} // namespace ikan
