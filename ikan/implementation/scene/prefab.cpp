//
//  prefab_serialiser.cpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#include "prefab.hpp"
#include "editor/content_browser_panel.hpp"
#include "editor/property_grid.hpp"
#include "renderer/utils/renderer.hpp"
#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  void Prefab::Serialize(Entity* entity, const std::string& file_path) {
    
  }
  
  Entity* Prefab::Deserialize(const std::string& file_path) {
    return nullptr;
  }
  
  void Prefab::Loader(bool* flag) {
    CHECK_WIDGET_FLAG(flag)
    
    ImGui::Begin("Prefab Loader", flag, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushID("Prefab Loader");
    
    static std::shared_ptr<Texture> prefab_tex = Renderer::GetTexture(DM::CoreAsset("textures/icons/asset.png"));
    const auto& relative_path = (std::filesystem::relative(CBP::GetCurrentDir(), CBP::GetRootDir())).string();
    float size = ImGui::GetWindowHeight() - 12.0f; // 12 just random number

    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    PropertyGrid::Image(INT2VOIDP(prefab_tex->GetRendererID()), { size, size });
    PropertyGrid::HoveredMsg(std::string("Drop the Game Object (Entity) on this icon to save the prefab \n"
                                         "- Prefab will be saved at current directory of Content Browser Panel \n"
                                         "- Current Durectory | " + CBP::GetCurrentDir().string()).c_str());

    ImGui::PopID();
    ImGui::End();

  }
  
} // namespace ikan
