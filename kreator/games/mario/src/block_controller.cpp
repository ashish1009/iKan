//
//  block_controller.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "block_controller.hpp"

namespace mario {
  
  static std::string GetTypeString(BlockType type) {
    switch (type) {
      case BlockType::Empty: return "Empty";
      case BlockType::Coin: return "Coin";
      case BlockType::PowerUp: return "PowerUp";
      default:
        IK_ASSERT(false);
    }
  }
  
  BlockController::BlockController(BlockType type, uint32_t count) : type_(type), count_(count) {
  }
  
  void BlockController::Create(Entity entity) {
  }
  
  void BlockController::Update(Timestep ts) {
  }
  
  void BlockController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
  }
  
  void BlockController::RenderGui() {
    ImGui::Text("Type | %s", GetTypeString(type_).c_str());
  }
  
  void BlockController::Copy(void* script) {
    if (!script) return;    
    BlockController* block_script = reinterpret_cast<BlockController*>(script);
    IK_ASSERT(block_script);
  }
  
  /// This structure Wraps the Block Data to store in Map for each Block
  struct BlockData {
    BlockType type;
    ScriptLoaderFn loader_fun;
    uint32_t item_count;
    BlockData() = default;
    BlockData(BlockType type, ScriptLoaderFn loader_fun, uint32_t item_count) : type(type), loader_fun(loader_fun), item_count(item_count) {}
  };
  
  struct BlockScriptManagerData {
    std::unordered_map<std::string, BlockData> block_map;
  };
  static std::shared_ptr<BlockScriptManagerData> data;
  
  void BlockScriptManager::Init() {
    data = std::make_shared<BlockScriptManagerData>();
    
    auto brick_loader_fn = ScriptLoader(mario::BlockController, mario::BlockType::Empty, 0);
    auto coin_loader_fn = ScriptLoader(mario::BlockController, mario::BlockType::Coin, 1);
    auto multi_coin_loader_fn = ScriptLoader(mario::BlockController, mario::BlockType::Coin, 10);
    auto powerup_coin_loader_fn = ScriptLoader(mario::BlockController, mario::BlockType::PowerUp, 0);
    
    data->block_map["Brick"] = {BlockType::Empty, brick_loader_fn, 0};
    data->block_map["CoinBonus"] = {BlockType::Coin, coin_loader_fn, 1};
    data->block_map["MultiCoinBonus"] = {BlockType::Coin, multi_coin_loader_fn, 10};
    data->block_map["PowerUpBonus"] = {BlockType::PowerUp, powerup_coin_loader_fn, 0};
  }
  
  void BlockScriptManager::Shutdown() { data.reset(); }
  ScriptLoaderFn BlockScriptManager::GetLoaderFn(const std::string& tag) { return data->block_map.at(tag).loader_fun; }
  BlockType BlockScriptManager::GetType(const std::string& tag) { return data->block_map.at(tag).type; }
  uint32_t BlockScriptManager::GetCount(const std::string& tag) { return data->block_map.at(tag).item_count; }
  
} // namespace mario
