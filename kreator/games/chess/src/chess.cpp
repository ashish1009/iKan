//
//  chess.cpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#include "chess.hpp"
#include "common.hpp"

namespace chess {
  
  Chess::Chess() {
    CHESS_LOG("Creating Chess Game Data ... ");
    Batch2DRenderer::AddLineData(50);
    Batch2DRenderer::AddQuadData(100);
  }
  
  Chess::~Chess() {
    CHESS_LOG("Destroying Chess Game Data ... ");
  }
  
  void Chess::Init(const std::shared_ptr<Scene> scene) {
    CHESS_LOG("Initialising Chess Game Data ... ");
    scene_ = scene;
  }
  
  void Chess::Update(Timestep ts) {
    if (!scene_) return;
    
    RenderBackgroundAndBorder();
    RenderChessGrids();
    RenderText();
  }
  
  void Chess::SetViewportSize(uint32_t width, uint32_t height) {
    viewport_width_ = width;
    viewport_height_ = height;
  }
  
  void Chess::LoadPrefab(const std::string &path, const Viewport &viewport) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;

    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport.height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport.mouse_pos_x - (float)viewport.width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport.mouse_pos_y - (float)viewport.height / 2) / zoom) + cam_data.position.y);

      int32_t x_pos_int = int32_t(x_pos / BlockSize);
      int32_t y_pos_int = int32_t(y_pos / BlockSize);
      
      x_pos = x_pos_int * BlockSize + BlockSize / 2;
      y_pos = y_pos_int * BlockSize + BlockSize / 2;
      
      Entity e = Prefab::Deserialize(path, scene_.get());
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
    }
  }
  
  void Chess::RenderChessGrids() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 0.6f};
    static const float z = -0.1f;
    Batch2DRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    for (int32_t row = 0; row <= MaxRows; row++) {
      Batch2DRenderer::DrawLine({0, row * BlockSize, z}, {MaxCols * BlockSize, row * BlockSize, z}, color);
    }
    for (int32_t col = 0; col <= MaxCols; col++) {
      Batch2DRenderer::DrawLine({col * BlockSize, 0, z}, {col * BlockSize, MaxRows * BlockSize, z}, color);
    }
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderBackgroundAndBorder() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static const std::shared_ptr<Texture> bg = Renderer::GetTexture(DM::ClientAsset("textures/background.png"));
    static const std::shared_ptr<Texture> border = Renderer::GetTexture(DM::ClientAsset("textures/border.png"));
    
    Batch2DRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    
    // Background
    static glm::mat4 bg_transform = Math::GetTransformMatrix({8, 8, -0.9}, {0, 0, 0}, {100, 100, 1});
    Batch2DRenderer::DrawQuad(bg_transform, bg);

    // Border
    static glm::mat4 border_transform = Math::GetTransformMatrix({8, 8, -0.5}, {0, 0, 0}, {19.2, 19.2, 1});
    Batch2DRenderer::DrawQuad(border_transform, border, { 0.641860485, 0.468707442, 0.468707442, 1 });
    
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderText() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static glm::vec2 size = {0.3f, 0.3f};
    static glm::vec4 color = { 0.7, 0.7, 0.7, 1};

    TextRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    for (int32_t row = 0;  row < MaxCols; row++) {
      TextRenderer::RenderText(std::to_string(row + 1), { -1, 0.5 + (BlockSize * row), 0.3f }, size, color);
    }
    for (int32_t col = 0;  col < MaxCols; col++) {
      TextRenderer::RenderText(std::string(1, 'A' + col), { 0.7f + (BlockSize * col), -1.0f, 0.3f }, size, color);
    }
    TextRenderer::EndBatch();
  }
  
} // namespace chess
