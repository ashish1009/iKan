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
  }
  
  void RuntimeItem::Shutdown() {
    data.reset();
  }
  
} // namespace mario
