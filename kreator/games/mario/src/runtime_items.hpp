//
//  runtime_items.hpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#pragma once

#include "sprite_manager.hpp"

namespace mario {
  
  using namespace ikan;
  
  enum class Items {
    Coin, Mushroom, Flower, Fireball, Star, Score
  };
  
  class CoinController : public ScriptableEntity {
  };
  
  class MushroomController : public ScriptableEntity {
  };
  
  class FlowerController : public ScriptableEntity {
  };
  
  class FireballController : public ScriptableEntity {
  };
  
  class StarController : public ScriptableEntity {
  };
  
  class ScoreController : public ScriptableEntity {
  };
  
  struct ItemData {
    std::string name;
    std::string scrip_name;
    ScriptLoaderFn loader_fun;
    
    ItemData() = default;
    ItemData(const std::string& name, const std::string& scrip_name, ScriptLoaderFn fun) : name(name), scrip_name(scrip_name), loader_fun(fun) { }
  };
  
  struct RuntimeItemData {
    std::unordered_map<Items, ItemData> item_map;
  };
  
  class RuntimeItem {
  public:
    /// This function initialise the runtime item scripts and data
    static void Init();
    /// This function sutdowns the runtime item scripts and data
    static void Shutdown();
    
    template <typename... Args>
    /// This function Spawns the Runtime Item by creating Entity and adding importatn Compoennts
    /// - Parameters:
    ///   - item: Item type
    ///   - scene: Scene pointer
    ///   - pos: Spawn Position
    ///   - args: Arguments required for Item Scrips....
    static void Spawn(Items item, Scene* scene, const glm::vec2& pos, Args&&... args) {
      static std::shared_ptr<Texture> items = SpriteManager::GetTexture(SpriteType::Items);

      // Create Runtime Entity
      auto run_time_entity = scene->CreateEntity(data_->item_map.at(item).name);
      
      // Update the Spawn Position
      auto& tc = run_time_entity.GetComponent<TransformComponent>();
      tc.UpdatePosition(glm::vec3(pos, 0.1f));

      if (item == Items::Score) {
        // Text
      }
      else {
        // Quad
      }
    }
    
  private:
    static std::shared_ptr<RuntimeItemData> data_;
  };
  
} // namespace mario
