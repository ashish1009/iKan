//
//  prefab_serialiser.cpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#include "prefab.hpp"
#include "scene/serialise_entity.hpp"
#include "editor/content_browser_panel.hpp"
#include "editor/property_grid.hpp"
#include "renderer/utils/renderer.hpp"
#include "renderer/graphics/texture.hpp"

#include <yaml-cpp/yaml.h>

namespace YAML {
  
  // yml converstions vec2
  template<> struct convert<glm::vec2> {
    static Node encode(const glm::vec2& rhs) {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.SetStyle(EmitterStyle::Flow);
      return node;
    }
    
    static bool decode(const Node& node, glm::vec2& rhs) {
      if (!node.IsSequence() or node.size() != 2)
        return false;
      
      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      return true;
    }
  };
  
} // namespace YAML

namespace ikan {

  void Prefab::Serialize(Entity* entity, const std::string& file_path) {
    IK_CORE_TRACE(LogModule::Prefab, "Serialising a Prefab");
    IK_CORE_TRACE(LogModule::Prefab, "  Path {0}", file_path);

    YAML::Emitter out;
    
    EntitySerialiser::SerialiseEntity(out, *entity);
    
    std::ofstream fout(file_path);
    fout << out.c_str();
  }
  
  Entity Prefab::Deserialize(const std::string& file_path, Scene* scene) {
    YAML::Node entity_data = YAML::LoadFile(file_path);
    std::string name;
    auto tag_component = entity_data["TagComponent"];
    name = tag_component["Tag"].as<std::string>();
    
    Entity deserialized_entity = scene->CreateEntity(name);
    IK_CORE_TRACE(LogModule::Prefab, "  Deserialising Entity");
    IK_CORE_TRACE(LogModule::Prefab, "  Name   {0}", name);

    EntitySerialiser::DeserislizeEntity(entity_data, deserialized_entity, scene);
    return deserialized_entity;
  }
  
  ImageData Prefab::GetImageData(const std::string &file_path) {
    ImageData data;
    YAML::Node entity_data = YAML::LoadFile(file_path);
    auto quad_component = entity_data["QuadComponent"];
    if (quad_component) {
      std::string texture_path = quad_component["Texture_Path"].as<std::string>();
      if (texture_path != "") {
        data.has_data = true;
        
        bool linear_edge = quad_component["Linear_Edge"].as<bool>();
        data.texture = Renderer::GetTexture(texture_path, linear_edge);
        
        int32_t num_coords = quad_component["Num_Coords"].as<int32_t>();
        if (num_coords > 0) {
          int32_t i = 0;
          auto coord = quad_component["Coords" + std::to_string(i)].as<glm::vec2>();
          auto sprite_size = quad_component["Sprite_Size" + std::to_string(i)].as<glm::vec2>();
          auto cell_size = quad_component["Cell_Size" + std::to_string(i)].as<glm::vec2>();
          auto sub_texture = SubTexture::CreateFromCoords(data.texture, coord, sprite_size, cell_size);
          
          data.uv0 = *(sub_texture->GetTexCoord() + 3);
          data.uv1 = *(sub_texture->GetTexCoord() + 1);
        }
      }
    } // if (quad_component)
    return data;
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
    PropertyGrid::DropConent([](void* data)
                             {
      if (!data) {
        IK_CORE_WARN(LogModule::Prefab, "Invalid Data");
        return;
      }
      
      Entity* entity = reinterpret_cast<Entity*>(data);
      if (!entity) {
        IK_CORE_WARN(LogModule::Prefab, "Entity can not be Casted from Data");
        return;
      }
      
      if (!entity->IsValidScene()) {
        IK_CORE_WARN(LogModule::Prefab, "Entity has Invalid Scene");
        return;
      }
      
      std::string file_path = CBP::GetCurrentDir().string() + "/" + entity->GetName() + prefab_extenstion_;
      Serialize(entity, file_path);
    });
    PropertyGrid::HoveredMsg(std::string("Drop the Game Object (Entity) on this icon to save the prefab \n"
                                         "- Prefab will be saved at current directory of Content Browser Panel \n"
                                         "- Current Durectory | " + CBP::GetCurrentDir().string()).c_str());

    ImGui::PopID();
    ImGui::End();
  }
  
} // namespace ikan
