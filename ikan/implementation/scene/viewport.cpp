//
//  viewport.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "viewport.hpp"

namespace ikan {
  
  Viewport::Viewport(const glm::vec4& fb_color) {
    FrameBuffer::Specification fb_spec;
    fb_spec.color = fb_color;
    fb_spec.attachments = {
      FrameBuffer::Attachments::TextureFormat::RGBA8,
      FrameBuffer::Attachments::TextureFormat::R32I,
      FrameBuffer::Attachments::TextureFormat::Depth24Stencil
    };
    
    framebuffer = FrameBuffer::Create(fb_spec);
  }
  
} // namespace ikan
