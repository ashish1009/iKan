//
//  runtime_items.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "runtime_items.hpp"
#include "common.hpp"

namespace mario {
  
  std::shared_ptr<RuntimeItemData> RuntimeItem::data_;
  
  void RuntimeItem::Init() {
    data_ = std::make_shared<RuntimeItemData>();
    
    static auto coin_script_loader = ScriptLoader(mario::CoinController);
    static auto mushroom_script_loader = ScriptLoader(mario::MushroomController);
    static auto flower_script_loader = ScriptLoader(mario::FlowerController);
    static auto fireball_script_loader = ScriptLoader(mario::FireballController);
    static auto star_script_loader = ScriptLoader(mario::FireballController);
    static auto score_script_loader = ScriptLoader(mario::ScoreController);

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
