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
    Coin, Mushroom, Flower, Fireball, BigFireball, Star, Score, InactiveBlock
  };
  
  class CoinController : public ScriptableEntity {
  public:
    void Create(Entity entity) override;
    void Update(Timestep ts) override;

  private:
    glm::vec2 top_pos_;
    float speed_ = 8.0f;
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
  public:
    ScoreController(int32_t score);
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    
  private:
    glm::vec2 top_pos_;
    float speed_ = 8.0f;
    int32_t score_ = 0;
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
        tc.AddPosition(Z, 0.1f);
        tc.UpdateScale({0.3, 0.3, 1.0f});

        MarioPrefab::AddScript<ScoreController>(&run_time_entity, data_->item_map.at(item).scrip_name, data_->item_map.at(item).loader_fun, std::forward<Args>(args)...);
      }
      else {
        MarioPrefab::AddQuad(&run_time_entity, SpriteManager::GetTexture(SpriteType::Items), {SpriteManager::GetItemSprite(item)});
        MarioPrefab::AddScript(&run_time_entity, data_->item_map.at(item).scrip_name, data_->item_map.at(item).loader_fun);
      }
    }
    
  private:
    static std::shared_ptr<RuntimeItemData> data_;
  };
  
} // namespace mario
