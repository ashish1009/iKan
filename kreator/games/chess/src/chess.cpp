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
    RenderChessGrids();
  }
  
  void Chess::SetViewportSize(uint32_t width, uint32_t height) {
    
  }
  
  void Chess::RenderChessGrids() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
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
  
} // namespace chess
