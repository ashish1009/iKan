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

    /// This function loads the font for freetype
    /// - Parameter font_file_path: path of font
    /// - Note: Load the Font before using any other API In client
    static void LoadFreetype(const std::string& font_file_path);

    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter cam_view_proj_mat: Camera View projection Matrix
    static void BeginBatch(const glm::mat4& cam_view_proj_mat);
    /// This function Ends the current batch by rendering all the vertex
    static void EndBatch();

    MAKE_PURE_STATIC(TextRenderer);
    
  private:
    /// This function flsh a single batch
    static void Flush();
    /// This function moves to next batch in single frame
    static void NextBatch();
  };
  
} // namespace ikan
