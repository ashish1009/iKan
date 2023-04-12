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

  static glm::mat4 still_camera_projection;
  
  RendererLayer::RendererLayer(GameType game_type)
  : Layer("Kreator"), game_data_(CreateGameData(game_type)), cbp_(DM::GetWorkspaceBasePath()) {
    KREATOR_LOG("Creating {0} Layer instance ... ", game_data_->GameName().c_str());
  }
  
  RendererLayer::~RendererLayer() {
    KREATOR_LOG("Destroying {0} Layer instance !!! ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Attach() {
    KREATOR_LOG("Attaching {0} Layer instance", game_data_->GameName().c_str());
    
    // Add paths in content browser panel
    cbp_.SetRootPath(game_data_->CbpRootDir());
    cbp_.AddFavouritPaths(game_data_->FavDirecotries());

    // Loading the Default Font for Text rendering
    TextRenderer::LoadFreetype(game_data_->RegularFontData().path);

    // Decorate the Imgui Change the font of imgui
    ImguiAPI::ChangeFont(game_data_->RegularFontData(), game_data_->BoldFontData());
    ImguiAPI::SetGreyThemeColors();
    
    const std::string& saved_scene = game_data_->SavedScene();
    if (StringUtils::GetExtensionFromFilePath(saved_scene) != saved_scene_extension_ or !OpenScene(saved_scene))
      NewScene(game_data_->GetScenePath() + "/NewScene" + saved_scene_extension_);
  }
  
  void RendererLayer::Detach() {
    KREATOR_LOG("Detaching {0} Layer instance ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Update(Timestep ts) {
    if (!active_scene_)
      return;

    viewport_.UpdateMousePos();
    editor_camera_.Update(ts);

    if (is_playing_) {
      Renderer::Clear(game_data_->GetBgColor());
      RenderScene(ts);
    }
    else {
      if (viewport_.IsFramebufferResized()) {
        viewport_.framebuffer->Resize(viewport_.width, viewport_.height);
        ResizeLayer(viewport_.width, viewport_.height);
      }

      viewport_.framebuffer->Bind();
      
      Renderer::Clear(viewport_.framebuffer->GetSpecification().color);
      RenderScene(ts);
      
      viewport_.framebuffer->Unbind();
    }
  }
  
  void RendererLayer::RenderScene(Timestep ts) {
    Batch2DRenderer::BeginBatch(editor_camera_.GetViewProjection());
    Batch2DRenderer::DrawQuad(glm::mat4(1.0f), {1, 1, 1, 1});
    Batch2DRenderer::EndBatch();
    
    // Text Renderer
    static glm::vec2 fixed_text_size = {0.3f, 0.3f};
    static glm::vec4 fixed_text_color = { 0.1, 0.1, 0.1, 1 };
    TextRenderer::BeginBatch(still_camera_projection);
    TextRenderer::RenderFixedViewText("(c) IKAN", { viewport_width_ - 60, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)),
                                      { 5.0f, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::EndBatch();
  }

  void RendererLayer::HandleEvents(Event& event) {
    editor_camera_.EventHandler(event);
    
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::KeyPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(RendererLayer::WindowResized));
  }

  bool RendererLayer::KeyPressed(KeyPressedEvent& event) {
    if (event.GetRepeatCount() > 0)
      return false;
    
    bool cmd = Input::IsKeyPressed(Key::LeftSuper) or Input::IsKeyPressed(Key::RightSuper);
    
    if (cmd) {
      switch (event.GetKeyCode()) {
        case Key::R: SetPlay(true); break;
          
        // File Manager
        case Key::N: NewScene();   break;
        case Key::X: CloseScene(); break;

        default: break;
      };
    }

    switch (event.GetKeyCode()) {
      case Key::Escape: SetPlay(false); break;
      default: break;
    }
    return false;
  }
  
  bool RendererLayer::WindowResized(WindowResizeEvent& event) {
    ResizeLayer(event.GetWidth(), event.GetHeight());
    return false;
  }
  
  void RendererLayer::ResizeLayer(uint32_t width, uint32_t height) {
    viewport_width_ = width;
    viewport_height_ = height;
    still_camera_projection = glm::ortho( 0.0f, (float)width, 0.0f, (float)height);
  }

  void RendererLayer::RenderGui() {
    if (is_playing_) {
    }
    else {
      ImguiAPI::StartDcocking();

      ShowMenu();
      GamePlayButton();
      
      if (active_scene_) {
        ImguiAPI::Framerate(&setting_.frame_rate.flag);
        Renderer::RenderStatsGui(&setting_.common_renderer_stats.flag);
        Renderer::Render2DStatsGui(&setting_.renderer_stats_2d.flag);
        viewport_.RenderGui(&setting_.viewport_data.flag);
        
        cbp_.RenderGui(&setting_.common_renderer_stats.flag);
        spm_.RenderGui();
        
        editor_camera_.RendererGui();
        
        if (active_scene_->IsEditing()) {
          SaveScene();
        }

        RenderViewport();
        SceneStateButton();
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
        if (StringUtils::GetExtensionFromFilePath(path) == "ikanScene")
          OpenScene(path);
        else
          IK_WARN("Invalid file for Scene {0}", path.c_str());
      });
    }

    viewport_.UpdateBound();

    ImGui::PopStyleVar();
    ImGui::PopID();
    ImGui::End();
  }
  
  void RendererLayer::ShowMenu() {
    if (ImGui::BeginMenuBar()) {
      ImguiAPI::Menu("File", true, [this]() {
        ImguiAPI::Menu("Scene", false, [this]() {
          ImguiAPI::MenuItem("New", "Cms + N", false, true, [this](){ NewScene(); });
          ImguiAPI::MenuItem("Close", "Cms + X", false, true, [this](){ CloseScene(); });
        }); // Scene
        
        ImGui::Separator();
        ImguiAPI::MenuItem("Exit", "Cmd + Q", false, true, []() {
          Application::Get().Close();
        }); // Exit
      }); // File
      
      ImguiAPI::Menu("Property", true, [this]() {
        ImguiAPI::Menu("Theme", true, [this]() {
          ImguiAPI::MenuItem("Light", nullptr, false, true, [this]() {
            viewport_.framebuffer->UpdateSpecificationColor({0.82f, 0.82f, 0.82f, 1.0f});
            ImguiAPI::SetLightThemeColors();
          });
          ImguiAPI::MenuItem("Dark", nullptr, false, true, [this]() {
            viewport_.framebuffer->UpdateSpecificationColor({0.08f, 0.08f, 0.08f, 1.0f});
            ImguiAPI::SetDarkThemeColors();
          });
          ImguiAPI::MenuItem("Grey", nullptr, false, true, [this]() {
            viewport_.framebuffer->UpdateSpecificationColor({0.18f, 0.18f, 0.18f, 1.0f});
            ImguiAPI::SetGreyThemeColors();
          });
          ImguiAPI::MenuItem("Light Grey", nullptr, false, true, [this]() {
            viewport_.framebuffer->UpdateSpecificationColor({0.25f, 0.25f, 0.25f, 1.0f});
            ImguiAPI::SetLightGreyThemeColors();
          });
        }); // Theme
      }); // Property
      
      ImguiAPI::Menu("Settings", true, [this]() {
        ImguiAPI::Menu("Scene", false, [this]() {
          ImguiAPI::MenuItem("Entity Panel", nullptr, spm_.GetSetting().scene_panel, true, [this](){
            spm_.GetSetting().scene_panel = (spm_.GetSetting().scene_panel) ? false : true;
          });
          ImguiAPI::MenuItem("Property Panel", nullptr, spm_.GetSetting().property_panel, true, [this](){
            spm_.GetSetting().scene_panel = (spm_.GetSetting().property_panel) ? false : true;
          });
        }); // Scene
        ImGui::Separator();
        
        FOR_EACH_SETTING {
          (setting_data + setting_idx)->ShowInMenu();
        }
      }); // Settings
      
      ImGui::EndMenuBar(); // ImGui::BeginMenuBar()
    } // if (ImGui::BeginMenuBar())
  }
  
  void RendererLayer::ShowSettings() {
    ImGui::Begin("Settings");

    FOR_EACH_SETTING {
      (setting_data + setting_idx)->CheckBox();
    }

    ImGui::Separator();
    PropertyGrid::CheckBox("Entity Panel", spm_.GetSetting().scene_panel, 3 * ImGui::GetWindowContentRegionMax().x / 4);
    PropertyGrid::CheckBox("Property Panel", spm_.GetSetting().property_panel, 3 * ImGui::GetWindowContentRegionMax().x / 4);

    ImGui::End();
  }
  
  void RendererLayer::GamePlayButton() {
    static std::shared_ptr<Texture> play_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/play.png"));
    ImGui::Begin("Game Play", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    float size = ImGui::GetWindowHeight() - 12.0f; // 12 just random number
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    // Button action
    if (PropertyGrid::ImageButton("Game Play", play_texture->GetRendererID(), { size, size })) {
      SetPlay(true);
    }
    PropertyGrid::HoveredMsg("Play Button for Game");

    ImGui::End();
  }
  
  void RendererLayer::SceneStateButton() {
    // Texture for Play and Pause button
    static std::shared_ptr<Texture> pause_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/pause.png"));
    static std::shared_ptr<Texture> play_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/play.png"));
    static std::shared_ptr<Texture> stop_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/stop.png"));
    
    // Play Pause Buttom
    ImGui::Begin("Scene Play/Pause/Stop", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushID("Scene Play/Pause/Stop");
    
    float size = ImGui::GetWindowHeight() - 12.0f; // 12 just random number

    // Update the texture id based on the state of scene
    uint32_t play_pause_tex_id = active_scene_->IsEditing() ? play_texture->GetRendererID() : pause_texture->GetRendererID();

    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 1.0f));

    // Play/Pause Button action
    if (PropertyGrid::ImageButton("Scene Play/Pause", play_pause_tex_id, { size, size })) {
      active_scene_->IsEditing() ? PlayScene() : EditScene();
    }
    PropertyGrid::HoveredMsg("Play Button for Scene (Debug Scene in play mode)");

    ImGui::SameLine();
    uint32_t stop_tex_id = stop_texture->GetRendererID();
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) + (size * 1.0f));
    
    // Stop Button action
    if (PropertyGrid::ImageButton("Scene Stop", stop_tex_id, { size, size })) {
      StopScene();
    }
    PropertyGrid::HoveredMsg("Play Button for Scene (Debug Scene in play mode)");

    ImGui::PopID();
    ImGui::End();
  }

  void RendererLayer::SetPlay(bool is_play) {
    is_playing_ = is_play;
    
    if (is_playing_) {
      ResizeLayer(Application::Get().GetWindowWidth(), Application::Get().GetWindowHeight());
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
    CloseScene();
    
    IK_TRACE(game_data_->GameName(), "Creating New Scene {0}", scene_path.c_str());
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
  }

  void RendererLayer::SaveScene() {
    if (!setting_ .save_scene.flag)
      return;
    
    static const float column_width = 120.0f;
    
    ImGui::Begin("Save File", &setting_.save_scene.flag);
    ImGui::PushID("Save File");

    const auto& relative_path = (std::filesystem::relative(cbp_.GetCurrentDir(), cbp_.GetRootDir())).string();
    PropertyGrid::ReadOnlyTextBox("Scene Directory", relative_path,
                                  "File will be saved at the Current directory in the active scene", column_width);

    static std::string file_name = "";
    bool modified = PropertyGrid::TextBox(file_name, "Scene Name", 2, column_width);
    
    if (modified) {
      std::string file_path = cbp_.GetCurrentDir().string() + "/" + file_name + ".ikanScene";
      
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
    IK_TRACE(game_data_->GameName(), "Opening saved scene from {0}", scene_path.c_str());
    
    CloseScene();
    
    editor_scene_ = std::make_shared<Scene>(scene_path);
    SceneSerializer serializer(editor_scene_.get());
    bool result = serializer.Deserialize(scene_path);
    
    active_scene_ = editor_scene_;
    spm_.SetSceneContext(active_scene_.get());
    game_data_->Init(active_scene_);
    
    return result;
  }
  
} // namespace kreator
