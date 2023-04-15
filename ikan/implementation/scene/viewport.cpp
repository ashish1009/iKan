//
//  viewport.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "viewport.hpp"
#include "imgui_api.hpp"
#include "property_grid.hpp"
#include "scene.hpp"
#include "core_entity.hpp"
#include <ImGuizmo.h>

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

  void Viewport::UpdateMousePos() {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    
    my = height - my;
    
    mouse_pos_x = (int32_t)mx;
    mouse_pos_y = (int32_t)my;
  }
  void Viewport::UpdateBound() {
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 min_bound   = ImGui::GetWindowPos();
    
    min_bound.x += cursor_pos.x;
    min_bound.y += cursor_pos.y;
    
    ImVec2 max_bound = { min_bound.x + window_size.x, min_bound.y + window_size.y };
    bounds[0] = { min_bound.x, min_bound.y };
    bounds[1] = { max_bound.x, max_bound.y };
  }
  
  bool Viewport::IsFramebufferResized() {
    const FrameBuffer::Specification& spec = framebuffer->GetSpecification();
    return width > 0 and height > 0 and (spec.width != width or spec.height != height);
  }
  
  void Viewport::UpdateHoveredEntity(Entity* current_selected_entity, Scene* scene) {
    if (!hovered) {
      hovered_entity_ = nullptr;
      return;
    }
    
    if (ImGuizmo::IsOver()) {
      hovered_entity_ = current_selected_entity;
      return;
    }
    
    // Get pixel from rednerer
    Renderer::GetEntityIdFromPixels(mouse_pos_x, mouse_pos_y, framebuffer->GetPixelIdIndex(), hovered_entity_id_);
    
    if (scene) {
      hovered_entity_ = (hovered_entity_id_ > (int32_t)scene->GetMaxEntityId()) ? nullptr : scene->GetEnitityFromId(hovered_entity_id_);
    }
  }

  void Viewport::RenderGui(bool *is_open) {
    CHECK_WIDGET_FLAG(is_open);

    ImGui::Begin("Viewport Data", is_open);
    ImGui::SetNextWindowContentSize(ImVec2(640.0f, 0.0f));
    ImGui::BeginChild("##RendererStats", ImVec2(0, ImGui::GetFontSize() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::Columns(8);

    ImGui::SetColumnWidth(0, 50);
    auto color = framebuffer->GetSpecification().color;
    if (ImGui::ColorEdit4("", &color.x, ImGuiColorEditFlags_NoInputs)) {
      framebuffer->UpdateSpecificationColor(color);
    }
    PropertyGrid::HoveredMsg("Framebuffer Background color");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(1, 90);
    ImGui::Text("%d x %d", mouse_pos_x, mouse_pos_y);
    PropertyGrid::HoveredMsg("Mouse relative position");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(2, 90);
    if (focused)
      ImGui::Text("Focused");
    ImGui::NextColumn();
    
    ImGui::SetColumnWidth(3, 90);
    if (hovered)
      ImGui::Text("Hovered");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(4, 90);
    ImGui::Text("%d x %d", width, height);
    PropertyGrid::HoveredMsg("Viewport Size");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(5, 100);
    ImGui::Text("%d", (int32_t)(hovered_entity_id_));
    ImGui::NextColumn();
    
    if (hovered_entity_) {
      std::string entity_name = hovered_entity_->GetComponent<TagComponent>().tag;
      ImGui::SetColumnWidth(6, 50);
      ImGui::Text("%d", (uint32_t)(*hovered_entity_));
      ImGui::NextColumn();
      
      ImGui::SetColumnWidth(7, 100);
      ImGui::Text("%s ", entity_name.c_str());
      ImGui::NextColumn();
    }
    else {
      ImGui::SetColumnWidth(6, 50);
      ImGui::NextColumn();
      
      ImGui::SetColumnWidth(7, 100);
      ImGui::NextColumn();
    }

    ImGui::Columns(1);
    
    ImGui::EndChild();
    ImGui::End();
  }
  
} // namespace ikan
