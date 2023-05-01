//
//  runtime_items.cpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#include "runtime_items.hpp"
#include "common.hpp"
#include "player.hpp"

namespace mario {
  
  static bool PowerupPlayerHitCheck(Entity* collided_entity, Entity* curr_entity, b2Contact* contact) {
    if (PlayerController::IsPlayer(collided_entity)) {
      contact->SetEnabled(false);
      PlayerController::Get()->SetPowerup();
      return true;
    }
    return false;
  }
    
  void CoinController::Create(Entity entity) {
    entity_ = entity;
    if (set_position_) {
      const auto& tc = entity_.GetComponent<TransformComponent>().Position();
      top_pos_ = {tc.x, tc.y + 5.0f};
      set_position_ = false;
    }
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

  void CoinController::Copy(void* script) {
    if (!script) return;
    CoinController* coin_script = reinterpret_cast<CoinController*>(script);
    IK_ASSERT(coin_script);
    set_position_ = coin_script->set_position_;
    top_pos_ = coin_script->top_pos_;
  }

  ScoreController::ScoreController(int32_t score) : score_(score) { }
  void ScoreController::Create(Entity entity) {
    entity_ = entity;
    if (set_position_) {
      MarioPrefab::AddText(&entity_, std::to_string(score_));
      
      const auto& tc = entity_.GetComponent<TransformComponent>().Position();
      top_pos_ = {tc.x, tc.y + 2.0f};
      set_position_ = false;
    }
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
  void ScoreController::Copy(void* script) {
    if (!script) return;
    ScoreController* score_script = reinterpret_cast<ScoreController*>(script);
    IK_ASSERT(score_script);
    set_position_ = score_script->set_position_;
    top_pos_ = score_script->top_pos_;
  }

  void MushroomController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    rbc_->fixed_rotation = true;
    rbc_->SetGravityScale(0.0f);
    
    CircleColliiderComponent* ccc = MarioPrefab::AddCircleCollider(&entity_);
    ccc->physics_mat.friction = 0.0f;
    
    entity_.scene_->AddBodyToPhysicsWorld(entity_, *rbc_);
    velocity_.y = entity_.scene_->Get2DWorldGravity().y * free_fall_factor;
  }
  
  void MushroomController::Update(Timestep ts) {
    if (going_right_ and std::abs(rbc_->velocity.x) < max_speed_) {
      rbc_->SetVelocity(velocity_);
    }
    else if (!going_right_ and std::abs(rbc_->velocity.x) < max_speed_) {
      rbc_->SetVelocity({-velocity_.x, velocity_.y});
    }
    
    if (destroy_)
      entity_.scene_->DestroyEntity(entity_);
  }
  
  void MushroomController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    if (destroy_) return;
    destroy_ = PowerupPlayerHitCheck(collided_entity, &entity_, contact);
    if (std::abs(contact_normal.y) < 0.1f) {
      going_right_ = contact_normal.x < 0.0f;
    }
  }
  
  void MushroomController::Copy(void* script) {
    if (!script) return;
    MushroomController* mushroom_script = reinterpret_cast<MushroomController*>(script);
    IK_ASSERT(mushroom_script);
    going_right_ = mushroom_script->going_right_;
    velocity_ = mushroom_script->velocity_;
  }
  
  void FlowerController::Create(Entity entity) {
    entity_ = entity;

    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    rbc_->is_sensor = true;
    rbc_->SetGravityScale(0.0f);

    MarioPrefab::AddCircleCollider(&entity_);
    entity_.scene_->AddBodyToPhysicsWorld(entity_, *rbc_);
  }
  
  void FlowerController::Update(Timestep ts) {
    if (destroy_)
      entity_.scene_->DestroyEntity(entity_);
  }
  void FlowerController::BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    if (destroy_) return;
    destroy_ = PowerupPlayerHitCheck(collided_entity, &entity_, contact);
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
