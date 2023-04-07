//
//  framebuffer.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "framebuffer.hpp"
#include "platform/open_gl/open_gl_framebuffer.hpp"

namespace ikan {
  
  std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBuffer::Specification& spec) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLFrameBuffer>(spec);
      case Renderer::Api::None:
      default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }

  FrameBuffer::Attachments::Attachments(std::initializer_list<Attachments::TextureFormat> attachments) : texture_formats(attachments) {}

}
