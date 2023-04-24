//
//  runtime_items.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "runtime_items.hpp"

namespace mario {
  
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
  static std::shared_ptr<RuntimeItemData> data;

  void RuntimeItem::Init() {
    data = std::make_shared<RuntimeItemData>();
    
    static auto coin_script_loader = ScriptLoader(mario::CoinController);
    static auto mushroom_script_loader = ScriptLoader(mario::MushroomController);
    static auto flower_script_loader = ScriptLoader(mario::FlowerController);
    static auto fireball_script_loader = ScriptLoader(mario::FireballController);
    static auto star_script_loader = ScriptLoader(mario::FireballController);
    static auto score_script_loader = ScriptLoader(mario::ScoreController);

    data->item_map[Items::Coin] = { "Block Coin", "mario::CoinController", coin_script_loader };
    data->item_map[Items::Mushroom] = { "Mushroom", "mario::MushroomController", mushroom_script_loader };
    data->item_map[Items::Flower] = { "Flower", "mario::FlowerController", flower_script_loader };
    data->item_map[Items::Fireball] = { "Fireball", "mario::FireballController", fireball_script_loader };
    data->item_map[Items::Star] = { "Star", "mario::StarController", star_script_loader };
    data->item_map[Items::Score] = { "Score", "mario::ScoreController", score_script_loader };
  }
  
  void RuntimeItem::Shutdown() {
    data.reset();
  }
  
} // namespace mario
