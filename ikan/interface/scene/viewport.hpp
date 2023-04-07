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
    DELETE_COPY_MOVE_CONSTRUCTORS(Viewport);
  };
  
} // namespace ikan
