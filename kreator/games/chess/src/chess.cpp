//
//  chess.cpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#include "chess.hpp"
#include "renderer_layer.hpp"

namespace chess {
  
  Chess::Chess() {
    CHESS_LOG("Creating Chess Game Data ... ");
    IK_ASSERT(BlockSize > 0);
    
    Batch2DRenderer::AddLineData(50);
    Batch2DRenderer::AddQuadData(100);

    // Setting Up the Initial camera position at the center of the board
    init_cam_pos_ = {(MaxCols / 2) * BlockSize, (MaxRows / 2) * BlockSize};
    
    // Default Player Data
    for (uint32_t i = 0; i < MaxPlayer; i++) {
      players_[i] = std::make_shared<Player>(GetColorString(Color(i)), Color(i));
    }
  }
  
  Chess::~Chess() {
    CHESS_LOG("Destroying Chess Game Data ... ");
  }
  
  void Chess::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    CHESS_LOG("Initialising Chess Game Data ... ");
    
    scene_ = scene;
    viewport_ = viewport;
    scene_->GetSetting().use_editor_camera = false;
    
    // Set the camera at the center of board in begining
    if (auto& cam_data = scene_->GetPrimaryCameraData(); cam_data.scene_camera) {
      cam_data.transform_comp->UpdatePosition({init_cam_pos_.x, init_cam_pos_.y, 0.0f});
    }
  }
  
  void Chess::Update(Timestep ts) {
    if (!scene_) return;
        
    // Update the local camera with scene camera
    auto& cam_data = scene_->GetPrimaryCameraData();
    camera_ = cam_data.scene_camera;
    if (!camera_) return;
    
    // Update the view projection matrix
    view_proj_ = camera_->GetProjection() * glm::inverse(cam_data.transform_comp->Transform());
    
    // Render Chess Data
    RenderBackgroundAndBorder();
    RenderText();
    RenderPlayerInfo();

    // Hopver the Block if playing
    if (is_playing_) {
      HighlightHoveredBlock();
    }
    else {
      RenderChessGrids();
    }
  }
  
  void Chess::EventHandler(Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseMovedEvent>(IK_BIND_EVENT_FN(Chess::MouseMoved));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(Chess::MouseClicked));
  }
  
  bool Chess::MouseMoved(MouseMovedEvent &mouse_move_event) {
    // Update the mouse position. If playing then From window size else from Viewport
    if (RendererLayer::IsPlaying()) {
      mouse_pos_.x = mouse_move_event.GetX();
      mouse_pos_.y = viewport_height_ - mouse_move_event.GetY();
    }
    else {
      mouse_pos_ = {viewport_->mouse_pos_x, viewport_->mouse_pos_y};
    }
    return false;
  }
  
  bool Chess::MouseClicked(MouseButtonPressedEvent &mouse_click_event) {
    // If Right Button clicked then return
    if (mouse_click_event.GetMouseButton() != MouseButton::ButtonLeft) return false;
    
    // If No Block Hovered then do nothing
    if (!hovered_block_) return false;
    
    // If Hovered Block is empty then return
    PieceRef hovered_piece = hovered_block_->GetPiece();
    if (!hovered_piece)  return false;
    
    // If selected block is of opponent color then update selected blocl
    if (hovered_piece->GetColor() == turn_) {
      selected_block_ = hovered_block_;
      
      PieceRef piece = selected_block_->GetPiece();
      std::vector<Position> possible_moves = piece->GetPossibleMoves();
    }
      
    return false;
  }
  
  void Chess::SetViewportSize(uint32_t width, uint32_t height) {
    viewport_width_ = width;
    viewport_height_ = height;
  }
  
  static void PrindBlockDataGui(Block* block) {
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionMax().x / 2);
    
    ImGui::Text("Block Row");
    ImGui::Text("Block Colum");
    
    PieceRef piece = block->GetPiece();
    if (piece) {
      ImGui::Text("Piece Type");
      ImGui::Text("Piece Color");
    }
    else {
      ImGui::Text("Empty Block");
    }
    
    ImGui::NextColumn();
    
    ImGui::Text("%d", block->GetRow());
    ImGui::Text("%d", block->GetCol());
    
    if (piece) {
      ImGui::Text("%s", GetPieceString(piece->GetPiece()).c_str());
      ImGui::Text("%s", GetColorString(piece->GetColor()).c_str());
    }
    ImGui::Separator();
    ImGui::Columns(1);
  }
  
  void Chess::RenderGui() {
    if (camera_) {
      ImGui::Begin("Zoom");
      camera_->ZoomWidget();
      ImGui::End();
    }
    
    ImGui::Begin("Chess");
    
    if (selected_block_) {
      ImGui::Text("Selected Block");
      ImGui::Separator();
      
      PrindBlockDataGui(selected_block_);
    }
    
    if (hovered_block_) {
      ImGui::Separator();

      ImGui::Text("Hovered Block");
      ImGui::Separator();
      
      PrindBlockDataGui(hovered_block_);
    }

    ImGui::End();
    
    if (!is_playing_) {
      for (int32_t i = 0; i < MaxPlayer; i++)
        players_[i]->RenderGui();
    }
  }
  
  void Chess::SetPlaying(bool playing_flag) {
    is_playing_ = playing_flag;
    if (is_playing_) {
      CreateBlocks();
    }
  }
  
  void Chess::LoadPrefab(const std::string &path) {
    glm::vec2 pos = GetBlockPositionFromMouse();
    if (pos.x == -1 or pos.y == -1) return;
    
    Entity e = Prefab::Deserialize(path, scene_.get());
    auto& tc = e.GetComponent<TransformComponent>();
    tc.UpdatePosition(X, pos.x);
    tc.UpdatePosition(Y, pos.y);
  }
  
  glm::vec2 Chess::GetBlockPositionFromMouse() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return {-1, -1 };
    
    float x_pos = -1, y_pos = -1;
    
    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_height_ / cam_data.scene_camera->GetZoom();
      x_pos = (mouse_pos_.x - (float)viewport_width_ / 2) / zoom;
      y_pos = (mouse_pos_.y - (float)viewport_height_ / 2) / zoom;
      
      // Getting position relating to camera position
      x_pos += cam_data.position.x;
      y_pos += cam_data.position.y;
      
      // If Position is outside board then return
      if (x_pos < 0 or x_pos > BlockSize * MaxCols) return {-1, -1};
      if (y_pos < 0 or y_pos > BlockSize * MaxRows) return {-1, -1};
      
      // Round of the values in INT
      int32_t x_pos_int = int32_t(x_pos / BlockSize);
      int32_t y_pos_int = int32_t(y_pos / BlockSize);
      
      // Getting position relating to block size
      x_pos = x_pos_int * BlockSize + BlockSize / 2;
      y_pos = y_pos_int * BlockSize + BlockSize / 2;
    }
    return {x_pos, y_pos};
  }
  
  void Chess::RenderChessGrids() {
    if (!camera_) return;
    
    static const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 0.6f};
    static const float z = -0.1f;
    Batch2DRenderer::BeginBatch(view_proj_);
    for (int32_t row = 0; row <= MaxRows; row++) {
      Batch2DRenderer::DrawLine({0, row * BlockSize, z}, {MaxCols * BlockSize, row * BlockSize, z}, color);
    }
    for (int32_t col = 0; col <= MaxCols; col++) {
      Batch2DRenderer::DrawLine({col * BlockSize, 0, z}, {col * BlockSize, MaxRows * BlockSize, z}, color);
    }
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderBackgroundAndBorder() {
    if (!camera_) return;
    
    static const std::shared_ptr<Texture> bg = Renderer::GetTexture(DM::ClientAsset("textures/background.png"));
    static const std::shared_ptr<Texture> border = Renderer::GetTexture(DM::ClientAsset("textures/border.png"));
    
    Batch2DRenderer::BeginBatch(view_proj_);
    
    // Background
    static glm::mat4 bg_transform = Math::GetTransformMatrix({init_cam_pos_.x, init_cam_pos_.y, -0.9}, {0, 0, 0}, {100, 100, 1});
    Batch2DRenderer::DrawQuad(bg_transform, bg);

    // Border
    static glm::mat4 border_transform = Math::GetTransformMatrix({init_cam_pos_.x, init_cam_pos_.y, -0.5}, {0, 0, 0}, {(BlockSize * (MaxCols + 1.6)), (BlockSize * (MaxRows + 1.6)), 1});
    Batch2DRenderer::DrawQuad(border_transform, border, { 0.641860485, 0.468707442, 0.468707442, 1 });
    
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderText() {
    if (!camera_) return;
    
    static glm::vec2 size = {0.3f, 0.3f};
    static glm::vec4 color = { 0.7, 0.7, 0.7, 1};

    TextRenderer::BeginBatch(view_proj_);
    for (int32_t row = 0;  row < MaxCols; row++) {
      TextRenderer::RenderText(std::to_string(row + 1), { -1, 0.5 + (BlockSize * row), 0.3f }, size, color);
    }
    for (int32_t col = 0;  col < MaxCols; col++) {
      TextRenderer::RenderText(std::string(1, 'A' + col), { 0.7f + (BlockSize * col), -1.0f, 0.3f }, size, color);
    }
    TextRenderer::EndBatch();
  }
  
  void Chess::HighlightHoveredBlock() {
    static const std::shared_ptr<Texture> hovered = Renderer::GetTexture(DM::ClientAsset("textures/hovered.png"));
    
    glm::vec2 pos = GetBlockPositionFromMouse();
    if (pos.x == -1 or pos.y == -1) {
      hovered_block_ = nullptr;
      return;
    }
    
    if (!camera_) return;

    Batch2DRenderer::BeginBatch(view_proj_);
    
    glm::mat4 transform = Math::GetTransformMatrix({pos.x, pos.y, 0.2}, glm::vec3(0.0f), {BlockSize, BlockSize, 1});
    Batch2DRenderer::DrawQuad(transform, hovered);
    
    Batch2DRenderer::EndBatch();

    int32_t col = (int32_t)((pos.x - BlockSize / 2) / BlockSize);
    int32_t row = (int32_t)((pos.y - BlockSize / 2) / BlockSize);

    
    // Update the Hovered block
    hovered_block_ = (BlockManager::blocks_[row][col]).get();
  }
  
  void Chess::CreateBlocks() {
    auto view = scene_->GetEntitesWith<TagComponent, TransformComponent>();
    for (auto e : view) {
      const auto& [tag_comp, transform_comp] = view.get<TagComponent, TransformComponent>(e);
      if (tag_comp.tag == "block") {
        const auto &p = transform_comp.Position();
        
        // Getting Row and column number from Position relative to block size
        float row = (p.y - BlockSize / 2) / BlockSize;
        float col = (p.x - BlockSize / 2) / BlockSize;
        
        // If Row and column out of bond
        IK_ASSERT(row >= 0 and row < MaxRows);
        IK_ASSERT(col >= 0 and col < MaxCols);
        
        BlockManager::blocks_[row][col] = std::make_shared<Block>(Position(row, col));
      }
    }
  
    for (auto e : view) {
      const auto& [tag_comp, transform_comp] = view.get<TagComponent, TransformComponent>(e);
      if (IsPieceTag(tag_comp.tag)) {
        const auto &p = transform_comp.Position();
        
        // Getting Row and column number from Position relative to block size
        float row = (p.y - BlockSize / 2) / BlockSize;
        float col = (p.x - BlockSize / 2) / BlockSize;
        
        // If Row and column out of bond
        IK_ASSERT(row >= 0 and row < MaxRows);
        IK_ASSERT(col >= 0 and col < MaxCols);
        
        // Get Color and piece of block
        Color color = GetColorFromTag(tag_comp.tag);
        PieceType piece = GetPieceFromTag(tag_comp.tag);

        BlockManager::blocks_[row][col]->SetData(color, piece);
      }
    }
  }
  
  void Chess::RenderPlayerInfo() {
    if (!camera_) return;
    
    static glm::vec2 size = {0.5f, 0.5f};
    
    glm::vec4 turn_color[2];
    turn_color[uint32_t(Color::Black)] = {0.0, 0.0, 0.0, 1.0};
    turn_color[uint32_t(Color::White)] = glm::vec4(1.0f);

    TextRenderer::BeginBatch(view_proj_);
    TextRenderer::RenderText(players_[0]->GetName(), { -8, (BlockSize * MaxRows), 0.2 }, size, turn_color[uint32_t(players_[0]->GetColor())]);
    TextRenderer::RenderText(players_[1]->GetName(), { (BlockSize * MaxCols) + 4, (BlockSize * MaxRows), 0.2 }, size,
                             turn_color[uint32_t(players_[1]->GetColor())]);
    TextRenderer::EndBatch();
    
    static std::shared_ptr<Texture> turn_texture = Renderer::GetTexture(DM::ClientAsset("textures/turn.png"));
    
    float x_pos[2];
    x_pos[uint32_t(Color::Black)] = -9;
    x_pos[uint32_t(Color::White)] = (BlockSize * MaxCols) + 3;
    
    Batch2DRenderer::BeginBatch(view_proj_);
    glm::mat4 transform = Math::GetTransformMatrix({x_pos[(uint32_t)turn_], (BlockSize * MaxRows) + 0.3, 0.2}, glm::vec3(0.0f), {1, 1, 1});
    Batch2DRenderer::DrawQuad(transform, turn_texture, turn_color[(uint32_t)turn_]);
    Batch2DRenderer::EndBatch();
  }
  
} // namespace chess
