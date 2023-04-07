//
//  viewport.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "viewport.hpp"
#include "imgui_api.hpp"
#include "property_grid.hpp"

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

  void Viewport::RenderGui(bool *is_open) {
    CHECK_WIDGET_FLAG(is_open);

    ImGui::Begin("Viewport Data", is_open);
    ImGui::SetNextWindowContentSize(ImVec2(20.0f, 0.0f));
    ImGui::BeginChild("##RendererStats", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

//    ImGui::Columns(1);

//    ImGui::SetColumnWidth(1, 100);
    auto color = framebuffer->GetSpecification().color;
    if (ImGui::ColorEdit4("", &color.x, ImGuiColorEditFlags_NoInputs)) {
      framebuffer->UpdateSpecificationColor(color);
    }
    PropertyGrid::HoveredMsg("Framebuffer Background color");

//    ImGui::Columns(1);
    
    ImGui::EndChild();
    ImGui::End();
  }
  
} // namespace ikan
