//
//  block_controller.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "block_controller.hpp"
#include "player.hpp"
#include "runtime_items.hpp"
#include "common.hpp"

namespace mario {
  
  static std::string GetTypeString(BlockType type) {
    switch (type) {
      case BlockType::Empty: return "Empty";
      case BlockType::Coin: return "Coin";
      case BlockType::PowerUp: return "PowerUp";
      case BlockType::Star: return "Star";
      default:
        IK_ASSERT(false);
    }
  }
  
  BlockController::BlockController(BlockType type, uint32_t count) : type_(type), count_(count) { }
  
  void BlockController::Create(Entity entity) {
    entity_ = entity;
    start_pos_ = glm::vec2(entity_.GetComponent<TransformComponent>().Position());
    end_pos_ = start_pos_ + glm::vec2(0.0f, 0.3f);
    
    rbc_ = &(GetComponent<RigidBodyComponent>());
    rbc_->SetGravityScale(0.0f);
    rbc_->fixed_rotation = true;

    MARIO_LOG("Creating Mario Block Controller");
  }
  
  void BlockController::Update(Timestep ts) {
    // Animation speed
    static constexpr float speed = 4.0f;
    
    if (change_body_type_) {
      rbc_->SetType(ikan::RigidBodyComponent::RbBodyType::Kinametic);
      animation_ = true;
      change_body_type_ = false;
    }
    // If Player Hit and Valid Animatuion block then Move the block little Up and then down to original Position
    if (animation_) {
      auto& tc = entity_.GetComponent<TransformComponent>();
      if (going_up_) {
        // Lift the block Up
        if (tc.Position().y < end_pos_.y) {
          rbc_->SetVelocity({0, speed});
        }
        // Send the block down
        else {
          going_up_ = false;
        }
      }
      else {
        // Move block to back to original Position
        if (tc.Position().y > start_pos_.y) {
          rbc_->SetVelocity({0, -speed});
        }
        // In case some margin left then Reset the Block Position to original
        else {
          rbc_->SetVelocity({0, 0});
          rbc_->SetType(ikan::RigidBodyComponent::RbBodyType::Static);
          tc.UpdatePosition(Y, start_pos_.y);
          going_up_ = true;
          animation_ = false;
        }
      }
    }
  }
  
  void BlockController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    PlayerController* pc = PlayerController::Get();
    if (active_ and pc and contact_normal.y < -0.8f) {
      change_body_type_ = true;
      PlayerHit(pc);
    }
  }
  
  void BlockController::PlayerHit(PlayerController *pc) {
    auto& tc = entity_.GetComponent<TransformComponent>();
    switch (type_) {
      case BlockType::Empty : {
        break;
      }
      case BlockType::Coin: {
        RuntimeItemManager::Spawn(Items::AnimatedCoin, entity_.scene_, {tc.Position().x, tc.Position().y + 1}, NoScore);
        RuntimeItemManager::Spawn(Items::Score, entity_.scene_, {tc.Position().x - 0.5, tc.Position().y + 1}, CoinScore);

        count_--;
        if (count_ == 0)
          SetInactive();

        break;
      }
      case BlockType::Star: {
        break;
      }
      case BlockType::PowerUp : {
        if (pc->IsSmall()) {
          RuntimeItemManager::Spawn(Items::Mushroom, entity_.scene_, {tc.Position().x, tc.Position().y + 1.0}, NoScore);
        }
        else {
          RuntimeItemManager::Spawn(Items::Flower, entity_.scene_, {tc.Position().x, tc.Position().y + 1.0}, NoScore);
        }
        SetInactive();
        break;
      }
      default:
        break;
    }
  }
  
  void BlockController::SetInactive() {
    auto& qc = entity_.GetComponent<QuadComponent>();
    qc.sprite.ClearSprites();
    qc.sprite.texture = SpriteManager::GetTexture(SpriteType::Items);
    qc.sprite.sprite_images = SpriteManager::GetItemSprite(Items::InactiveBlock);
        
    active_ = false;
  }
  
  void BlockController::RenderGui() {
    ImGui::Text("Type       | %s", GetTypeString(type_).c_str());
    ImGui::Text("Active     | %s", active_ ? "True" : "False");
    ImGui::Text("Animation  | %s", animation_ ? "True" : "False");
    ImGui::Text("Going Up   | %s", going_up_ ? "True" : "False");
    ImGui::Text("Item Count | %d", count_);
    
    ImGui::Separator();
    ImGui::Text("Start Pos  | %f x %f", start_pos_.x, start_pos_.y);
    ImGui::Text("End   Pos  | %f x %f", end_pos_.x, end_pos_.y);
  }
  
  void BlockController::Copy(void* script) {
    if (!script) return;    
    BlockController* block_script = reinterpret_cast<BlockController*>(script);
    IK_ASSERT(block_script);
    
    going_up_ = block_script->going_up_;
    animation_ = block_script->animation_;
    start_pos_ = block_script->start_pos_;
    end_pos_ = block_script->end_pos_;
    active_ = block_script->active_;
    type_ = block_script->type_;
    count_ = block_script->count_;
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
    auto start_loader_fn = ScriptLoader(mario::BlockController, mario::BlockType::Star, 0);

    data->block_map["Brick"] = {BlockType::Empty, brick_loader_fn, 0};
    data->block_map["CoinBonus"] = {BlockType::Coin, coin_loader_fn, 1};
    data->block_map["MultiCoinBonus"] = {BlockType::Coin, multi_coin_loader_fn, 10};
    data->block_map["PowerUpBonus"] = {BlockType::PowerUp, powerup_coin_loader_fn, 0};
    data->block_map["Start"] = {BlockType::Star, start_loader_fn, 0};
    
    MARIO_LOG("Initialised the Block Script Manager");
  }
  
  void BlockScriptManager::Shutdown() {
    data.reset();
    MARIO_LOG("Destroyed the Block Script Manager");
  }
  ScriptLoaderFn BlockScriptManager::GetLoaderFn(const std::string& tag) { return data->block_map.at(tag).loader_fun; }
  BlockType BlockScriptManager::GetType(const std::string& tag) { return data->block_map.at(tag).type; }
  uint32_t BlockScriptManager::GetCount(const std::string& tag) { return data->block_map.at(tag).item_count; }
  
} // namespace mario
