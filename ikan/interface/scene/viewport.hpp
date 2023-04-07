//
//  viewport.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

#include "renderer/graphics/framebuffer.hpp"

namespace ikan {
  
  struct Viewport {
    std::shared_ptr<FrameBuffer> framebuffer;

    Viewport(const glm::vec4& fb_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    /// This function render the viewport information in gui
    /// - Parameter is_open: flag to show or hide the widget
    void RenderGui(bool *is_open = nullptr);
    DELETE_COPY_MOVE_CONSTRUCTORS(Viewport);
  };
  
} // namespace ikan
