//
//  open_gl_framebuffer.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

#include "renderer/graphics/framebuffer.hpp"

namespace ikan {
  
  /// Implementation to store Open GL Framebuffer
  class OpenGLFrameBuffer : public FrameBuffer {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - parameter spec: Framebuffer current specification
    OpenGLFrameBuffer(const Specification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLFrameBuffer();
    
    /// This function binds the current FrameBuffer to the renderer
    void Bind() const override;
    /// This function unbinds the current FrameBuffer from the renderer
    void Unbind() const override;
    
    /// This function resizes the Framebuffer
    /// - parameters:
    ///   - width: New width of FB
    ///   - height: New height of FB
    void Resize(uint32_t width, uint32_t height) override;
    
    /// This funciton updates the specification color
    /// - Parameter color: color
    void UpdateSpecificationColor(const glm::vec4& color) override;
    
    /// This function returns the Frame buffer specification
    const Specification& GetSpecification() const override { return specification_; }
    /// This function returns the Renderer ID
    RendererID GetRendererId() const override { return renderer_id_; }
    /// This function returns the Depth Attachment ID
    RendererID GetDepthAttachmentId() const override { return depth_attachment_id_; }
    /// This function returns the Color Attachment iDs
    const std::vector<RendererID>& GetColorAttachmentIds() const override { return color_attachment_ids_; }
    /// This function returns the pixel id (RED_ID) index stored in fremebuffer
    uint32_t GetPixelIdIndex() const override { return pixel_id_index_; }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLFrameBuffer);
    
  private:
    /// This function invalidate the frame buffer on creation on resize
    void Invalidate();
    
    RendererID renderer_id_ = 0;
    Specification specification_;
    
    // Stores the attachments of Framebuffer
    std::vector<Attachments::TextureFormat> color_specifications_;
    Attachments::TextureFormat depth_specification_ = Attachments::TextureFormat::None;
    
    // Stores the attachment ID for both color and depth
    std::vector<RendererID> color_attachment_ids_;
    RendererID depth_attachment_id_ = 0;
    
    uint32_t pixel_id_index_ = 0;
  };
  
} // namespace ikan
