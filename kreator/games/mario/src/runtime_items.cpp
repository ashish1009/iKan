//
//  runtime_items.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "runtime_items.hpp"
#include "common.hpp"

namespace mario {
  
  void CoinController::Create(Entity entity) {
    entity_ = entity;
    const auto& tc = entity_.GetComponent<TransformComponent>().Position();
    top_pos_ = {tc.x, tc.y + 5.0f};
  }
  
  void CoinController::Update(Timestep ts) {
    auto& tc = entity_.GetComponent<TransformComponent>();
    if (tc.Position().y < top_pos_.y) {
      tc.AddPosition(Y, ts * speed_);
      tc.AddScale(X, -fmod((2.5f * ts), -1.0f));
    }
    else {
      entity_.scene_->DestroyEntity(entity_);
    }
  }
  
  ScoreController::ScoreController(int32_t score) : score_(score) { }
  void ScoreController::Create(Entity entity) {
    entity_ = entity;
    MarioPrefab::AddText(&entity_, std::to_string(score_));
    
    const auto& tc = entity_.GetComponent<TransformComponent>().Position();
    top_pos_ = {tc.x, tc.y + 2.0f};
  }
  void ScoreController::Update(Timestep ts) {
    auto& tc = entity_.GetComponent<TransformComponent>();
    if (tc.Position().y < top_pos_.y) {
      tc.AddPosition(Y, (ts * speed_));
    }
    else {
      entity_.scene_->DestroyEntity(entity_);
    }
  }

  std::shared_ptr<RuntimeItemData> RuntimeItem::data_;
  
  void RuntimeItem::Init() {
    data_ = std::make_shared<RuntimeItemData>();
    
    static auto coin_script_loader = ScriptLoader(mario::CoinController);
    static auto mushroom_script_loader = ScriptLoader(mario::MushroomController);
    static auto flower_script_loader = ScriptLoader(mario::FlowerController);
    static auto fireball_script_loader = ScriptLoader(mario::FireballController);
    static auto star_script_loader = ScriptLoader(mario::FireballController);
    static auto score_script_loader = ScriptLoader(mario::ScoreController, 100); // Default score is 100

    data_->item_map[Items::Coin] = { "Block Coin", "mario::CoinController", coin_script_loader };
    data_->item_map[Items::Mushroom] = { "Mushroom", "mario::MushroomController", mushroom_script_loader };
    data_->item_map[Items::Flower] = { "Flower", "mario::FlowerController", flower_script_loader };
    data_->item_map[Items::Fireball] = { "Fireball", "mario::FireballController", fireball_script_loader };
    data_->item_map[Items::Star] = { "Star", "mario::StarController", star_script_loader };
    data_->item_map[Items::Score] = { "Score", "mario::ScoreController", score_script_loader };
    
    MARIO_LOG("Initialised the Runtime Items Manager");
  }
  
  void RuntimeItem::Shutdown() {
    data_.reset();
    MARIO_LOG("Destroyed the Runtime Items Manager");
  }

} // namespace mario
