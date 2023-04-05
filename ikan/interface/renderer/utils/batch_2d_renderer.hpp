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
  };
  
} // namespace ikan
