//
//  block_controller.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "block_controller.hpp"

namespace mario {
  
  /// This structure Wraps the Block Data to store in Map for each Block
  struct BlockData {
    BlockType type;
    ScriptLoaderFn loader_fun;
    uint32_t item_count;
    BlockData(BlockType type, ScriptLoaderFn loader_fun, uint32_t item_count) : type(type), loader_fun(loader_fun), item_count(item_count) {}
  };
  
  struct BlockScriptManagerData {
    std::unordered_map<std::string, BlockData> block_map;
  };
  static std::shared_ptr<BlockScriptManagerData> data;
  
  void BlockScriptManager::Init() {
    data = std::make_shared<BlockScriptManagerData>();
  }
  
  void BlockScriptManager::Shutdown() { data.reset(); }
  ScriptLoaderFn BlockScriptManager::GetLoaderFn(const std::string& tag) { return data->block_map.at(tag).loader_fun; }
  BlockType BlockScriptManager::GetType(const std::string& tag) { return data->block_map.at(tag).type; }
  uint32_t BlockScriptManager::GetCount(const std::string& tag) { return data->block_map.at(tag).item_count; }
  
} // namespace mario
