//
//  batch_2d_renderer.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
  
  class Batch2DRenderer {
  public:
    /// This function initialzes the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    /// - Parameters:
    ///   - max_quads: Max Quad to render in one batch
    ///   - max_cirlces: Max Circles to render in one batch
    ///   - max_lines: Max Lines to render in one batch
    static void Initialise(uint32_t max_quads, uint32_t max_cirlces, uint32_t max_lines);
    /// This functio dhutdown the batch renderer and destoy all the buffer reserved on initialize
    static void Shutdown();
    /// This function Dump the data in logs
    static void LogData();
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter cam_view_proj_mat: Camera View projection Matrix
    static void BeginBatch(const glm::mat4& cam_view_proj_mat);
    /// This function Ends the current batch by rendering all the vertex
    static void EndBatch();

    /// This funcition Adds the quads renderer data to prev allocaged data
    /// - Parameter max_quads: max quad to be renderered in single batch
    static void AddQuadData(uint32_t max_quads);
    /// This funcition Adds the circle renderer data to prev allocaged data
    /// - Parameter max_circles: max circle to be renderered in single batch
    static void AddCircleData(uint32_t max_circles);
    /// This funcition Adds the lines renderer data to prev allocaged data
    /// - Parameter max_lines: max circle to be renderered in single batch
    static void AddLineData(uint32_t max_lines);

    MAKE_PURE_STATIC(Batch2DRenderer);
    
  private:
    /// This function moves to next batch in single frame
    static void NextBatch();
    /// This function flsh a single batch
    static void Flush();

  };
  
} // namespace ikan
