//
//  renderer_layer.cpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#include "renderer_layer.hpp"

namespace kreator {
  
#define KREATOR_LOG(...) IK_TRACE(game_data_->GameName(), __VA_ARGS__)
  
#define FOR_EACH_SETTING \
  SettingWrapper* setting_data = ((SettingWrapper*)&setting_); \
  static uint32_t loop_limit = sizeof(Setting) / sizeof(SettingWrapper); \
  for (uint32_t setting_idx = 0; setting_idx < loop_limit; setting_idx++) \

#define SETTING_TOGGLE(name, flag) \
  if (ImGui::MenuItem(name, nullptr, flag, true)) { \
    flag = (flag) ? false : true; \
  }
  
  RendererLayer::RendererLayer(GameType game_type)
  : Layer("Kreator"), game_data_(CreateGameData(game_type)) {
    KREATOR_LOG("Creating {0} Layer instance ... ", game_data_->GameName().c_str());
  }
  
  RendererLayer::~RendererLayer() {
    KREATOR_LOG("Destroying {0} Layer instance !!! ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Attach() {
    KREATOR_LOG("Attaching {0} Layer instance", game_data_->GameName().c_str());
    
    // Add paths in content browser panel
    CBP::SetRootPath(game_data_->CbpRootDir());
    CBP::AddFavouritPaths(game_data_->FavDirecotries());
    CBP::AddAssetPaths(game_data_->AssetDirecotries());

    // Loading the Default Font for Text rendering
    TextRenderer::LoadFreetype(game_data_->RegularFontData().path);

    // Decorate the Imgui Change the font of imgui
    ImguiAPI::ChangeFont(game_data_->RegularFontData(), game_data_->BoldFontData());
    ImguiAPI::SetGreyThemeColors();
    
    const std::string& saved_scene = game_data_->SavedScene();
    if (StringUtils::GetExtensionFromFilePath(saved_scene) != saved_scene_extension_ or !OpenScene(saved_scene)) {
      NewScene(game_data_->GetScenePath() + "/NewScene" + saved_scene_extension_); 
      active_scene_->SetType(Scene::_3D);
    }
  }
  
  void RendererLayer::Detach() {
    KREATOR_LOG("Detaching {0} Layer instance ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Update(Timestep ts) {
    if (!active_scene_)
      return;

    viewport_.UpdateMousePos();

    if (is_playing_) {
      Renderer::Clear(game_data_->GetBgColor());
      RenderScene(ts);
    }
    else {
      if (viewport_.IsFramebufferResized()) {
        viewport_.framebuffer->Resize(viewport_.width, viewport_.height);
        ResizeData(viewport_.width, viewport_.height);
      }

      viewport_.framebuffer->Bind();
      
      Renderer::Clear(viewport_.framebuffer->GetSpecification().color);
      RenderScene(ts);
      
      if (!active_scene_->GetSetting().use_editor_camera)
        RenderGrid();

      if (active_scene_->GetType() == Scene::Type::_2D) {
        if (!game_data_->IsPlaying()) {
          SelectEntities();
        }
      }
      
      OverlayRender();
      
      viewport_.UpdateHoveredEntity(spm_.GetSelectedEntity(), active_scene_.get());
      viewport_.framebuffer->Unbind();
    }
  }
  
  void RendererLayer::RenderScene(Timestep ts) {
    active_scene_->Update(ts);
    game_data_->Update(ts);
    
    // Text Renderer
    static glm::vec2 fixed_text_size = {0.3f, 0.3f};
    static glm::vec4 fixed_text_color = { 0.1, 0.1, 0.1, 1};
    TextRenderer::BeginBatch(FixedCamera::projection);
    TextRenderer::RenderFixedViewText("(c) IKAN", { viewport_width_ - 80, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), { 5.0f, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::EndBatch();
  }

  void RendererLayer::HandleEvents(Event& event) {
    if (active_scene_)
      active_scene_->EventHandler(event);
    
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::KeyPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(RendererLayer::WindowResized));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::MouseButtonPressed));
  }

  bool RendererLayer::KeyPressed(KeyPressedEvent& event) {
    if (event.GetRepeatCount() > 0)
      return false;
    
    bool ctrl = Input::IsKeyPressed(Key::LeftControl) or Input::IsKeyPressed(Key::RightControl);
    bool cmd = Input::IsKeyPressed(Key::LeftSuper) or Input::IsKeyPressed(Key::RightSuper);
    bool left_shift = Input::IsKeyPressed(Key::LeftShift);

    if (cmd) {
      switch (event.GetKeyCode()) {
        case Key::T: PlayScene();   break;
        case Key::R: SetPlay(true); break;
          
        // File Manager
        case Key::N: NewScene();   break;
        case Key::X: CloseScene(); break;

        default: break;
      };
    }
    
    if (ctrl) {
      switch (event.GetKeyCode()) {
        case Key::Q: viewport_.guizmo_type = -1;                              break;
        case Key::W: viewport_.guizmo_type = ImGuizmo::OPERATION::TRANSLATE;  break;
        case Key::E: viewport_.guizmo_type = ImGuizmo::OPERATION::ROTATE;     break;
        case Key::R: viewport_.guizmo_type = ImGuizmo::OPERATION::SCALE;      break;
        default: break;
      }
    }
    
    if (left_shift) {
      switch (event.GetKeyCode()) {
        case Key::D:          DuplicateSelectedEntities();  break;
        case Key::Backspace:  DeleteSelectedEntities();     break;
        case Key::Escape:     ClearSelectedEntities();      break;
          
        case Key::Left:       MoveEntities(Left);   break;
        case Key::Right:      MoveEntities(Right);  break;
        case Key::Up:         MoveEntities(Up);     break;
        case Key::Down:       MoveEntities(Down);   break;
          
        default: break;
      } // switch (e.GetKeyCode())
    } // if (shift)

    switch (event.GetKeyCode()) {
      case Key::Escape: SetPlay(false); break;
      default: break;
    }
    return false;
  }
  
  bool RendererLayer::WindowResized(WindowResizeEvent& event) {
    ResizeData(event.GetWidth(), event.GetHeight());
    return false;
  }
  
  bool RendererLayer::MouseButtonPressed(MouseButtonPressedEvent& e) {
    if (e.GetMouseButton() == MouseButton::ButtonLeft) {
      if (viewport_.IsMouseInsideViewport())
        spm_.SetSelectedEntity(viewport_.hovered_entity_);
    }
    return false;
  }

  void RendererLayer::ResizeData(uint32_t width, uint32_t height) {
    viewport_width_ = width;
    viewport_height_ = height;
    
    active_scene_->SetViewport(width, height);
    game_data_->SetViewportSize(width, height);
    
    FixedCamera::SetViewport(width, height);
  }

  void RendererLayer::RenderGui() {
    if (is_playing_) {
    }
    else {
      ImguiAPI::StartDcocking();

      ShowMenu();
      
      if (active_scene_) {
        active_scene_->RenderGui();

        ImguiAPI::Framerate(&setting_.frame_rate.flag);
        Renderer::RenderStatsGui(&setting_.common_renderer_stats.flag);
        Renderer::Render2DStatsGui(&setting_.renderer_stats_2d.flag);
        viewport_.RenderGui(&setting_.viewport_data.flag);
        
        CBP::RenderGui(&setting_.content_browser_panel.flag);
        spm_.RenderGui();
                
        if (active_scene_->IsEditing()) {
          SaveScene();
          Prefab::Loader(&setting_.prefab_loader.flag);
        }

        RenderViewport();
        SceneStateButton();
        SceneRendererType();
        ShowSettings();
      }

      ImguiAPI::EndDcocking();
    }
  }
  
  void RendererLayer::RenderViewport() {
    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Kreator Viewport");
    ImGui::PushID("Kreator Viewport");
    
    viewport_.focused = ImGui::IsWindowFocused();
    viewport_.hovered = ImGui::IsWindowHovered();
    
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    viewport_.width = viewport_panel_size.x;
    viewport_.height = viewport_panel_size.y;

    size_t textureID = viewport_.framebuffer->GetColorAttachmentIds().at(0);
    ImGui::Image((void*)textureID, viewport_panel_size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    if (active_scene_->IsEditing()) {
      PropertyGrid::DropConent([this](const std::string& path)
                               {
        if (StringUtils::GetExtensionFromFilePath(path) == saved_scene_extension_) {
          OpenScene(path);
        }
        else if (StringUtils::GetExtensionFromFilePath(path) == prefab_extenstion_) {
          const auto& cam_data = active_scene_->GetPrimaryCameraData();
          if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
            float zoom = viewport_.height / cam_data.scene_camera->GetZoom();
            float x_pos = (viewport_.mouse_pos_x - (float)viewport_.width / 2) / zoom;
            float y_pos = (viewport_.mouse_pos_y - (float)viewport_.height / 2) / zoom;
            
            Entity e = Prefab::Deserialize(path, active_scene_.get());
            auto& tc = e.GetComponent<TransformComponent>();
            tc.UpdatePosition(X, x_pos);
            tc.UpdatePosition(Y, y_pos);
          }
        }
        else {
          IK_WARN(game_data_->GameName(), "Invalid file for Scene {0}", path.c_str());
        }
      });
      OnImguizmoUpdate();
    }

    viewport_.UpdateBound();

    ImGui::PopStyleVar();
    ImGui::PopID();
    ImGui::End();
  }
  
  void RendererLayer::ShowMenu() {
    if (ImGui::BeginMenuBar()) {
      ImguiAPI::Menu("File", true, [this]() {
        ImguiAPI::Menu("Scene", true, [this]() {
          if (ImGui::MenuItem("New", "Cmd + N"))    NewScene();
          if (ImGui::MenuItem("Close", "Cmd + X"))  CloseScene();
        }); // Scene
        
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Cmd + Q"))     Application::Get().Close();
      }); // File
      
      ImguiAPI::Menu("Property", true, [this]() {
        ImguiAPI::Menu("Theme", true, [this]() {
          if (ImGui::MenuItem("Light", nullptr)) {
            viewport_.framebuffer->UpdateSpecificationColor({0.82f, 0.82f, 0.82f, 1.0f});
            ImguiAPI::SetLightThemeColors();
          }
          if (ImGui::MenuItem("Dark", nullptr)) {
            viewport_.framebuffer->UpdateSpecificationColor({0.08f, 0.08f, 0.08f, 1.0f});
            ImguiAPI::SetDarkThemeColors();
          }
          if (ImGui::MenuItem("Grey", nullptr)) {
            viewport_.framebuffer->UpdateSpecificationColor({0.18f, 0.18f, 0.18f, 1.0f});
            ImguiAPI::SetGreyThemeColors();
          }
          if (ImGui::MenuItem("Light Grey", nullptr)) {
            viewport_.framebuffer->UpdateSpecificationColor({0.25f, 0.25f, 0.25f, 1.0f});
            ImguiAPI::SetLightGreyThemeColors();
          }
        }); // Theme
      }); // Property
      
      ImguiAPI::Menu("Settings", true, [this]() {
        ImguiAPI::Menu("Scene", false, [this]() {
          SETTING_TOGGLE("Editor Camera", active_scene_->GetSetting().editor_camera);
          SETTING_TOGGLE("Entity Panel", spm_.GetSetting().scene_panel);
          SETTING_TOGGLE("Property Panel", spm_.GetSetting().property_panel);
        }); // Scene
        ImGui::Separator();
        SETTING_TOGGLE("Show Setting Widget", show_setting_);
        ImGui::Separator();

        FOR_EACH_SETTING {
          (setting_data + setting_idx)->ShowInMenu();
        }
      }); // Settings
      
      ImGui::EndMenuBar(); // ImGui::BeginMenuBar()
    } // if (ImGui::BeginMenuBar())
  }
  
  void RendererLayer::ShowSettings() {
    CHECK_WIDGET_FLAG(&show_setting_);
    
    ImGui::Begin("Settings", &show_setting_);

    PropertyGrid::CheckBox("Use Editor Camera", active_scene_->GetSetting().use_editor_camera, 3 * ImGui::GetWindowContentRegionMax().x / 4);
    if (active_scene_->GetSetting().use_editor_camera)
      PropertyGrid::CheckBox("Show Editor Camera", active_scene_->GetSetting().editor_camera, 3 * ImGui::GetWindowContentRegionMax().x / 4);
    
    ImGui::Separator();
    PropertyGrid::CheckBox("Entity Panel", spm_.GetSetting().scene_panel, 3 * ImGui::GetWindowContentRegionMax().x / 4);
    PropertyGrid::CheckBox("Property Panel", spm_.GetSetting().property_panel, 3 * ImGui::GetWindowContentRegionMax().x / 4);

    ImGui::Separator();

    FOR_EACH_SETTING {
      (setting_data + setting_idx)->CheckBox();
    }

    ImGui::End();
  }
  
  void RendererLayer::SceneStateButton() {
    // Texture for Play and Pause button
    static std::shared_ptr<Texture> scene_pause_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/pause.png"));
    static std::shared_ptr<Texture> scene_play_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/simulate.png"));
    static std::shared_ptr<Texture> scene_stop_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/stop.png"));
    static std::shared_ptr<Texture> game_play_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/play.png"));

    ImGui::Begin("Play/Pause/Stop", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushID("Play/Pause/Stop");
    
    float size = ImGui::GetWindowHeight() - 12.0f; // 12 just random number

    // Scene Play Pause buttorn
    uint32_t play_pause_tex_id = active_scene_->IsEditing() ? scene_play_texture->GetRendererID() : scene_pause_texture->GetRendererID();
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 2.0f));
    if (PropertyGrid::ImageButton("Scene Play/Pause", play_pause_tex_id, { size, size })) {
      active_scene_->IsEditing() ? PlayScene() : EditScene();
    }
    PropertyGrid::HoveredMsg("Play Button for Scene (Debug Scene in play mode)");

    // Game Play Button
    ImGui::SameLine();
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
    if (PropertyGrid::ImageButton("Game Play", game_play_texture->GetRendererID(), { size, size })) {
      SetPlay(true);
    }
    PropertyGrid::HoveredMsg("Play Button for Game");

    // Scene Stop Button
    ImGui::SameLine();
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) + (size * 1.0f));
    if (PropertyGrid::ImageButton("Scene Stop", scene_stop_texture->GetRendererID(), { size, size })) {
      StopScene();
    }
    PropertyGrid::HoveredMsg("Play Button for Scene (Debug Scene in play mode)");

    ImGui::PopID();
    ImGui::End();
  }

  void RendererLayer::SceneRendererType() {
    // Play Pause Buttom
    ImGui::Begin("Scene Renderer Type", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushID("Scene Renderer Type");
    
    Scene::Type current_type = active_scene_->GetType();
    Scene::Type new_type = Scene::Type(PropertyGrid::ComboDrop("Scene Renderer Type", { "2D" , "3D" }, (uint32_t)current_type,
                                                               3 * ImGui::GetWindowContentRegionMax().x / 5));
    if (new_type != current_type) {
      active_scene_->SetType((Scene::Type)new_type);
    }
    
    ImGui::PopID();
    ImGui::End();
  }

  void RendererLayer::SetPlay(bool is_play) {
    is_playing_ = is_play;
    
    if (is_playing_) {
      Application::Get().MaximizeWindow();
      PlayScene();
    }
    else {
      EditScene();
    }
  }

  void RendererLayer::PlayScene() {
    active_scene_ = Scene::Copy(editor_scene_);
    spm_.SetSceneContext(active_scene_.get());
    
    game_data_->Init(active_scene_);
    game_data_->SetPlaying(true);

    active_scene_->PlayScene();
  }
  
  void RendererLayer::EditScene() {
    game_data_->SetPlaying(false);
    active_scene_->EditScene();
  }

  void RendererLayer::StopScene() {
    active_scene_ = editor_scene_;
    spm_.SetSceneContext(active_scene_.get());

    game_data_->Init(active_scene_);
    game_data_->SetPlaying(false);

    active_scene_->EditScene();
  }
  
  void RendererLayer::NewScene(const std::string& scene_path) {
    IK_TRACE(game_data_->GameName(), "Creating New Scene {0}", scene_path.c_str());
    NewSceneImpl(scene_path);
  }

  void RendererLayer::NewSceneImpl(const std::string& scene_path) {
    CloseScene();
    
    editor_scene_ = std::make_shared<Scene>(scene_path);
    active_scene_ = editor_scene_;
    spm_.SetSceneContext(active_scene_.get());
  }

  void RendererLayer::CloseScene() {
    if (!active_scene_)
      return;
    
    IK_TRACE(game_data_->GameName(), "Closing Scene {0}", active_scene_->GetName().c_str());
    active_scene_.reset();
    active_scene_ = nullptr;
    
    editor_scene_.reset();
    editor_scene_ = nullptr;
    
    spm_.SetSelectedEntity(nullptr);
  }

  void RendererLayer::SaveScene() {
    if (!setting_ .save_scene.flag)
      return;
        
    ImGui::Begin("Save File", &setting_.save_scene.flag);
    ImGui::PushID("Save File");

    static std::string file_name = "";
    const auto& relative_path = (std::filesystem::relative(CBP::GetCurrentDir(), CBP::GetRootDir())).string();
    std::string hint = "Scene will be saved at " + CBP::GetCurrentDir().string();
    bool modified = PropertyGrid::TextBox(file_name, "Scene Name", 2, 100.0f, hint.c_str());
    PropertyGrid::HoveredMsg(hint.c_str());
    
    if (modified) {
      std::string file_path = CBP::GetCurrentDir().string() + "/" + file_name + ".ikanScene";
      IK_TRACE(game_data_->GameName(), "Saving Scene at {0}", file_path.c_str());
      if (!file_path.empty()) {
        active_scene_->SetFilePath(file_path);
        SceneSerializer serializer(active_scene_.get());
        serializer.Serialize(file_path);
      }
    }
    
    ImGui::PopID();
    ImGui::End();
  }
  
  const bool RendererLayer::OpenScene(const std::string& scene_path) {
    NewSceneImpl(scene_path);
    active_scene_->SetViewport(viewport_width_, viewport_height_);
    
    IK_TRACE(game_data_->GameName(), "Opening saved scene from {0}", scene_path.c_str());
    SceneSerializer serializer(editor_scene_.get());
    
    bool result = serializer.Deserialize(scene_path);
    game_data_->Init(active_scene_);
    
    return result;
  }
  
  void RendererLayer::RenderGrid() {
    const auto& cd = active_scene_->GetPrimaryCameraData();
    if (cd.scene_camera)
      cd.scene_camera->RenderGrids(1000, {0.6, 0.6, 0.6, 0.2}, cd.transform_matrix, cd.position);
  }
  
  void RendererLayer::SelectEntities() {
    if (!viewport_.IsMouseInsideViewport()) {
      ClearSelectedEntities();
      return;
    }
    
    if (!Input::IsKeyPressed(Key::LeftShift) or Input::IsKeyPressed(Key::LeftControl))
      return;

    static bool first_clicked = true;
    
    static glm::vec2 initial_mouse_position_ = glm::vec2(0.0f);
    static glm::vec2 final_mouse_position_ = glm::vec2(0.0f);
    
    static glm::vec2 initial_block_position_ = glm::vec2(0.0f);
    static glm::vec2 final_block_position_ = glm::vec2(0.0f);

    const auto& cd = active_scene_->GetPrimaryCameraData();
    float zoom = cd.scene_camera->GetZoom();
    float aspect_ratio = cd.scene_camera->GetAspectRatio();

    if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft)) {
      if (first_clicked) {
        ClearSelectedEntities();
        
        first_clicked = false;
        initial_mouse_position_ = { viewport_.mouse_pos_x, viewport_.mouse_pos_y };
        initial_block_position_ = {
          viewport_.mouse_pos_x - ((float)viewport_.width / 2),
          viewport_.mouse_pos_y - ((float)viewport_.height / 2)
        };
        initial_block_position_ *= ((zoom * aspect_ratio) / viewport_.width);
      }
      final_mouse_position_ = { viewport_.mouse_pos_x, viewport_.mouse_pos_y };
      final_block_position_ = {
        viewport_.mouse_pos_x - ((float)viewport_.width / 2),
        viewport_.mouse_pos_y - ((float)viewport_.height / 2)
      };
      final_block_position_ *= ((zoom * aspect_ratio) / viewport_.width);
      
      // Render the outline rectangle
      Batch2DRenderer::BeginBatch(active_scene_->GetPrimaryCameraData().scene_camera->GetProjection() *
                                  glm::inverse(active_scene_->GetPrimaryCameraData().transform_matrix));
      Batch2DRenderer::DrawRect({initial_block_position_.x + cd.position.x, initial_block_position_.y + cd.position.y, 0.1},
                                {final_block_position_.x + cd.position.x, final_block_position_.y + cd.position.y, 0.1},
                                {1, 1, 1, 1});
      Batch2DRenderer::EndBatch();
    }
    if (Input::IsMouseButtonReleased(MouseButton::ButtonLeft)) {
      if (!first_clicked) {
        // Store entites present in selected entitity
        float min_x = std::min(initial_mouse_position_.x, final_mouse_position_.x);
        float max_x = std::max(initial_mouse_position_.x, final_mouse_position_.x);
        float min_y = std::min(initial_mouse_position_.y, final_mouse_position_.y);
        float max_y = std::max(initial_mouse_position_.y, final_mouse_position_.y);
        
        for (float i_x = min_x; i_x <= max_x; i_x ++) {
          for (float i_y = min_y; i_y <= max_y; i_y++) {
            // Get pixel from rednerer
            int32_t pixel = -1;
            
            Renderer::GetEntityIdFromPixels(i_x, i_y, viewport_.framebuffer->GetPixelIdIndex(), pixel);
#if 0
            IK_TRACE(game_data_->GameName(), "X : {0}, Y : {1}, Pixel : {2}", i_x, i_y, pixel);
#endif
            if (pixel <= (int32_t)active_scene_->GetMaxEntityId()) {
              if (selected_entities_.find((entt::entity)pixel) == selected_entities_.end()){
                selected_entities_[(entt::entity)pixel] = active_scene_->GetEnitityFromId(pixel);
              }
            }
          }
        }
        
        // TODO: Do it while adding to vector? or keep separate???
        HighlightSelectedEntities(true);
      }
      first_clicked = true;
    }
  }
  
  void RendererLayer::ClearSelectedEntities() {
    HighlightSelectedEntities(false);
    selected_entities_.clear();
  }
  
  void RendererLayer::HighlightSelectedEntities(bool enable) {
    for (auto& [entt, entity] : selected_entities_) {
      if(!entity) continue;
      auto& qc = entity->GetComponent<QuadComponent>();
      (enable) ? qc.color.a -=0.2f : qc.color.a +=0.2f;
    }
  }
  
  void RendererLayer::DeleteSelectedEntities() {
    for (auto& [entt, entity] : selected_entities_) {
      if (spm_.GetSelectedEntity() and *(spm_.GetSelectedEntity()) == *entity) {
        spm_.SetSelectedEntity(nullptr);
      }
      active_scene_->DestroyEntity(*entity);
    }
    selected_entities_.clear();
  }
  
  void RendererLayer::DuplicateSelectedEntities() {
    HighlightSelectedEntities(false);
    for (auto& [entt, entity] : selected_entities_) {
      if (entity)
        [[maybe_unused]] auto e = active_scene_->DuplicateEntity(*entity);
    }
    HighlightSelectedEntities(true);
  }
  
  void RendererLayer::MoveEntities(Direction direction) {
    for (auto& [entt, entity] : selected_entities_) {
      if(!entity) continue;
      
      auto& tc = entity->GetComponent<TransformComponent>();
      switch (direction) {
        case Down:      tc.AddPosition(Y, - 1.0f);     break;
        case Up:        tc.AddPosition(Y, 1.0f);       break;
        case Right:     tc.AddPosition(X, 1.0f);       break;
        case Left:      tc.AddPosition(X, - 1.0f);     break;
        default: break;
      }
    }
  }

  void RendererLayer::OverlayRender() {
    if (setting_.show_collider.flag) {
      RenderColliders();
    }
  }
  
  void RendererLayer::RenderColliders() {
    static const glm::vec4 collider_color = {0.0f, 1.0f, 0.0f, 1.0f};
    bool camera_found = false;
    
    if (active_scene_->IsEditorCameraEnabled()) {
      Batch2DRenderer::BeginBatch(active_scene_->GetEditorCamera().GetViewProjection());
      camera_found = true;
    }
    else {
      const auto& cd = active_scene_->GetPrimaryCameraData();
      if (cd.scene_camera)
        camera_found = true;
      
      if (!camera_found)
        return;
      
      Batch2DRenderer::BeginBatch(cd.scene_camera->GetProjection() * glm::inverse(cd.transform_matrix));
    }
    
    // Box coilider
    auto box_view = active_scene_->GetEntitesWith<TransformComponent, Box2DColliderComponent>();
    for (auto entity : box_view) {
      auto [tc, bcc] = box_view.get<TransformComponent, Box2DColliderComponent>(entity);
      glm::vec3 p = tc.Position() + glm::vec3(bcc.offset, 0.001f);
      glm::vec3 s = tc.Scale() * glm::vec3((bcc.size * 2.0f), 1.0f); // We need diameter
      Batch2DRenderer::DrawRect(Math::GetTransformMatrix(p, tc.Rotation(), s), collider_color);
    }
  
    // Circle Collider
    auto circle_view = active_scene_->GetEntitesWith<TransformComponent, CircleColliiderComponent>();
    for (auto entity : circle_view) {
      auto [tc, ccc] = circle_view.get<TransformComponent, CircleColliiderComponent>(entity);
      glm::vec3 p = tc.Position() + glm::vec3(ccc.offset, 0.001f);
      glm::vec3 s = tc.Scale() * glm::vec3(ccc.radius * 2.0f); // We need diameter
      Batch2DRenderer::DrawCircle(Math::GetTransformMatrix(p, {0, 0, 0}, s), collider_color, 0.05f);
    }

    // Pill Box Collider
    auto pbc_view = active_scene_->GetEntitesWith<TransformComponent, PillBoxColliderComponent>();
    for (auto entity : pbc_view) {
      auto [tc, pbc] = pbc_view.get<TransformComponent, PillBoxColliderComponent>(entity);
      pbc.RecalculateColliders();
      
      // Box
      const auto& bcc = pbc.bcc;
      glm::vec3 bcc_p = tc.Position() + glm::vec3(bcc.offset, 0.001f);
      glm::vec3 bcc_s = glm::vec3((bcc.size * 2.0f), 1.0f); // We need diameter
      Batch2DRenderer::DrawRect(Math::GetTransformMatrix(bcc_p, tc.Rotation(), bcc_s), collider_color);

      // Top Circle
      const auto& top_ccc = pbc.top_ccc;
      glm::vec3 top_ccc_p = tc.Position() + glm::vec3(top_ccc.offset, 0.001f);
      glm::vec3 top_ccc_s = glm::vec3(top_ccc.radius * 2.0f); // We need diameter
      Batch2DRenderer::DrawCircle(Math::GetTransformMatrix(top_ccc_p, {0, 0, 0}, top_ccc_s), collider_color, 0.05f);

      // Bottom Circle
      const auto& bottom_ccc = pbc.bottom_ccc;
      glm::vec3 bottom_ccc_p = tc.Position() + glm::vec3(bottom_ccc.offset, 0.001f);
      glm::vec3 bottom_ccc_s = glm::vec3(bottom_ccc.radius * 2.0f); // We need diameter
      Batch2DRenderer::DrawCircle(Math::GetTransformMatrix(bottom_ccc_p, {0, 0, 0}, bottom_ccc_s), collider_color, 0.05f);
    }
    Batch2DRenderer::EndBatch();
  }

  void RendererLayer::OnImguizmoUpdate() {
    Entity* selected_entity = spm_.GetSelectedEntity();
    if (selected_entity and viewport_.guizmo_type != -1) {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();
      
      float window_width = (float)ImGui::GetWindowWidth();
      float window_height = (float)ImGui::GetWindowHeight();
      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);
      
      // Entity transform
      auto& tc = selected_entity->GetComponent<TransformComponent>();
      glm::mat4 transform = tc.Transform();
      
      // Snapping
      bool snap = Input::IsKeyPressed(Key::LeftControl);
      float snap_value = 0.5f; // Snap to 0.5m for translation/scale
      
      // Snap to 45 degrees for rotation
      if (viewport_.guizmo_type == ImGuizmo::OPERATION::ROTATE)
        snap_value = 45.0f;
      
      float snap_values[3] = { snap_value, snap_value, snap_value };
      
      if (!active_scene_->IsEditorCameraEnabled()) {
        // Camera
        const auto& cd = active_scene_->GetPrimaryCameraData();
        const glm::mat4& camera_projection = cd.scene_camera->GetProjection();
        
        if (cd.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
          const glm::mat4& camera_view = glm::inverse(Math::GetTransformMatrix({cd.position.x, cd.position.y, cd.position.z + 1.0f},
                                                                               cd.transform_comp->Rotation(),
                                                                               cd.transform_comp->Scale()));
          ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)viewport_.guizmo_type,
                               ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snap_values : nullptr);
        }
        else {
          const glm::mat4& camera_view = glm::inverse(cd.transform_comp->Transform());
          ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)viewport_.guizmo_type,
                               ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snap_values : nullptr);
        }
      } else {
        // Camera
        const EditorCamera& editor_camera = active_scene_->GetEditorCamera();
        
        const glm::mat4& camera_projection = editor_camera.GetProjection();
        const glm::mat4& camera_view = editor_camera.GetView();
        
        ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)viewport_.guizmo_type,
                             ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snap_values : nullptr);
      }
      
      if (ImGuizmo::IsUsing()) {
        glm::vec3 translation, rotation, scale;
        Math::DecomposeTransform(transform, translation, rotation, scale);
        
        glm::vec3 deltaRotation = rotation - tc.Rotation();
        tc.UpdatePosition(translation);
        tc.UpdateRotation(tc.Rotation() + deltaRotation);
        tc.UpdateScale(scale);
      } // if (ImGuizmo::IsUsing())
    } // if (selected_entity and viewport_.guizmo_type != -1)
  }
  
} // namespace kreator
