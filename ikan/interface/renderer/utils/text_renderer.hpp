//
//  text_renderer.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
  
  class TextRenderer {
  public:
    /// This function initializes the Text renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    static void Initialise();
    /// This function Shuts down on Text the Batch Renderer. Delete all the allocated Data
    static void Shutdown();
    /// This function Dump the data in logs
    static void LogData();

    MAKE_PURE_STATIC(TextRenderer);
  };
  
} // namespace ikan
