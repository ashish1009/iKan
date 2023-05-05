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
    
  uint32_t FireballController::fire_ball_count_ = 0;
  void FireballController::Create(Entity entity) {
    entity_ = entity;

    entity_.GetComponent<TransformComponent>().UpdateScale({0.5, 0.5, 1.0f});
    
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    rbc_->fixed_rotation = true;
    rbc_->SetGravityScale(0.0f);
    
    CircleColliiderComponent* ccc = MarioPrefab::AddCircleCollider(&entity_);
    ccc->physics_mat.friction = 0.0f;
    
    // Get the direction of Fireball with reference of player
    going_right_ = PlayerController::Get()->IsRight();

    if (inc_counter_)  {
      fire_ball_count_++;
      inc_counter_ = false;

      entity_.scene_->AddBodyToPhysicsWorld(entity_, *rbc_);
    }
  }
  
  void FireballController::Update(Timestep ts) {
    life_time_ -= ts;
    if (life_time_ <= 0) {
      destroy_ = true;
    }
    
    if (destroy_) {
      entity_.GetComponent<QuadComponent>().sprite.sprite_images = SpriteManager::GetItemSprite(Items::BigFireball);
      entity_.GetComponent<TransformComponent>().UpdateScale({1, 1, 1});

      destroy_time_ -= ts;
      rbc_->SetVelocity({0, 0});

      if (destroy_time_ <= 0) {
        entity_.scene_->DestroyEntity(entity_);
        fire_ball_count_ --;
      }
      return;
    }

    CheckOnGround();

    if (going_right_) {
      velocity_.x = fireball_speed_;
    } else {
      velocity_.x = -fireball_speed_;
    }

    if (on_ground_) {
      acceleration_.y = 12.5f;
      velocity_.y = 12.5f;
    } else {
      acceleration_.y = entity_.scene_->Get2DWorldGravity().y * free_fall_factor;
    }
    
    velocity_.y += acceleration_.y * ts * 2.0f;
    velocity_.y = std::max(std::min(velocity_.y, terminal_velocity_.y), -terminal_velocity_.y);
    
    rbc_->SetVelocity(velocity_);
    rbc_->SetAngularVelocity(0.0f);
  }
  
  void FireballController::PreSolve(Entity *collided_entity, b2Contact *contact, const glm::vec2 &contact_normal) {
    if (std::abs(contact_normal.x) > 0.5f) {
      destroy_ = true;
    }
  }

  void FireballController::CheckOnGround() {
    on_ground_ = entity_.scene_->CheckOnGround(&entity_, 0.4f, -0.33);
  }
  
  void FireballController::RenderGui() {
    ImGui::Text(" On Ground         | %s", on_ground_ ? "True" : "False");
  }
  
  void FireballController::Copy(void *script) {
    if (!script) return;
    FireballController* fire_script = reinterpret_cast<FireballController*>(script);
    IK_ASSERT(fire_script);
    
    inc_counter_ = fire_script->inc_counter_;
    on_ground_ = fire_script->on_ground_;
    going_right_ = fire_script->going_right_;
    destroy_ = fire_script->destroy_;
    
    destroy_time_ = fire_script->destroy_time_;
    life_time_ = fire_script->life_time_;
    
    acceleration_ = fire_script->acceleration_;
    velocity_ = fire_script->velocity_;
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
