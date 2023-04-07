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
  
  RendererLayer::RendererLayer(GameType game_type) : Layer("Kreator"), game_data_(CreateGameData(game_type)) {
    KREATOR_LOG("Creating {0} Layer instance ... ", game_data_->GameName().c_str());
  }
  
  RendererLayer::~RendererLayer() {
    KREATOR_LOG("Destroying {0} Layer instance !!! ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Attach() {
    KREATOR_LOG("Attaching {0} Layer instance", game_data_->GameName().c_str());
    
    // Loading the Default Font for Text rendering
    TextRenderer::LoadFreetype(game_data_->RegularFontData().path);
  }
  
  void RendererLayer::Detach() {
    KREATOR_LOG("Detaching {0} Layer instance ", game_data_->GameName().c_str());
  }
  
  void RendererLayer::Update(Timestep ts) {
    viewport_.UpdateMousePos();

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
    TextRenderer::RenderFixedViewText("(c) IKAN", { viewport_width_ - 90, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)),
                                      { 5.0f, 5.0f, 0.3f }, fixed_text_size, fixed_text_color);
    TextRenderer::EndBatch();
  }

  void RendererLayer::HandleEvents(Event& event) {
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
      ShowSettings();
      
      ImguiAPI::Framerate(&setting_.frame_rate.flag);
      Renderer::RenderStatsGui(&setting_.common_renderer_stats.flag);
      Renderer::Render2DStatsGui(&setting_.renderer_stats_2d.flag);
      viewport_.RenderGui(&setting_.viewport_data.flag);

      RenderViewport();

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
    
    viewport_.UpdateBound();

    ImGui::PopStyleVar();
    ImGui::PopID();
    ImGui::End();
  }
  
  void RendererLayer::ShowMenu() {
    if (ImGui::BeginMenuBar()) {
      ImguiAPI::Menu("File", true, []() {
        ImguiAPI::Menu("Scene", false, []() {
          
        }); // Scene
        
        ImGui::Separator();
        ImguiAPI::MenuItem("Exit", "Cmd + Q", false, true, []() {
          Application::Get().Close();
        }); // Exit
      }); // File
      
      ImguiAPI::Menu("Property", true, []() {
        ImguiAPI::Menu("Theme", false, []() {
          
        }); // Theme
      }); // Property
      
      ImguiAPI::Menu("Settings", true, [this]() {
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
  
  void RendererLayer::SetPlay(bool is_play) {
    is_playing_ = is_play;
    
    if (is_playing_) {
      ResizeLayer(Application::Get().GetWindowWidth(), Application::Get().GetWindowHeight());
    }
  }

} // namespace kreator
