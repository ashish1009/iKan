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
    virtual ~CoinController() = default;
    
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void Copy(void* script) override;

  private:
    static constexpr float speed_ = 8.0f;
    glm::vec2 top_pos_;
    bool set_position_ = true;
  };
  
  class ScoreController : public ScriptableEntity {
  public:
    ScoreController(int32_t score);
    virtual ~ScoreController() = default;
    
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void Copy(void* script) override;

  private:
    static constexpr float speed_ = 8.0f;
    glm::vec2 top_pos_;
    int32_t score_ = 0;
    bool set_position_ = true;
  };

  class MushroomController : public ScriptableEntity {
  public:
    virtual ~MushroomController() = default;
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& normal) override;
    void Copy(void* script) override;
    
  private:
    static constexpr float max_speed_ = 8.0f;
    static constexpr float free_fall_factor = 1.0f;
    
    bool going_right_ = true;
    bool destroy_ = false;
    glm::vec2 velocity_ = { 4.0f, 0.0f };
  };
  
  class FlowerController : public ScriptableEntity {
  public:
    virtual ~FlowerController() = default;
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    
  private:
    bool destroy_ = false;
  };
  
  class FireballController : public ScriptableEntity {
  public:
    static const uint32_t MaxFireballs = 4;
    static uint32_t fire_ball_count_;

    void Create(Entity entity) override;
  };
  
  class StarController : public ScriptableEntity {
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
      if (item == Items::Fireball and FireballController::fire_ball_count_ >= FireballController::MaxFireballs ) {
        return;
      }

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
        MarioPrefab::AddQuad(&run_time_entity, SpriteManager::GetTexture(SpriteType::Items), SpriteManager::GetItemSprite(item));
        MarioPrefab::AddScript(&run_time_entity, data_->item_map.at(item).scrip_name, data_->item_map.at(item).loader_fun);
      }
    }
    
  private:
    static std::shared_ptr<RuntimeItemData> data_;
  };
  
} // namespace mario
