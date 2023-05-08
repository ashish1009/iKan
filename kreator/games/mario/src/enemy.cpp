//
//  enemy.cpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#include "enemy.hpp"
#include "common.hpp"
#include "player.hpp"
#include "sprite_manager.hpp"
#include "runtime_items.hpp"

namespace mario {
  
  void EnemyController::Initialize(Entity* entity, RigidBodyComponent* rbc) {
    // Disbale Gravity on Enemy
    rbc->SetGravityScale(0.0f);
    
    // Free fall with scene gravity
    acceleration_.y = entity->scene_->Get2DWorldGravity().y * free_fall_factor;
  }
  
  void EnemyController::CheckOnGround(Entity* entity) {
    static float inner_enemy_width = 0.6f;
    float y_val = -(height_ / 2);
    y_val -= 0.02f;
    
    on_ground_ = entity->scene_->CheckOnGround(entity, inner_enemy_width, y_val);
  }
  
  void EnemyController::Update(Timestep ts, Entity* entity, RigidBodyComponent* rbc) {
    // Check on Ground
    CheckOnGround(entity);
    
    // Update velocities
    if (on_ground_) {
      acceleration_.y = 0;
      velocity_.y = 0;
      
      if (going_right_) {
        velocity_.x = walk_speed_;
      }
      else if (!going_right_) {
        velocity_.x = -walk_speed_;
      }
    } else {
      acceleration_.y = entity->scene_->Get2DWorldGravity().y * free_fall_factor;
      velocity_.x = 0.0f;
    }

    velocity_.y += acceleration_.y * ts * 2.0f;
    
    velocity_.x = std::max(std::min(velocity_.x, terminal_velocity_.x), -terminal_velocity_.x);
    velocity_.y = std::max(std::min(velocity_.y, terminal_velocity_.y), -terminal_velocity_.y);
    
    auto& rb = entity->GetComponent<RigidBodyComponent>();
    rb.SetVelocity(velocity_);
    rb.SetAngularVelocity(0.0f);
  }
  
  void EnemyController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal, Entity* entity) {
    if (is_dead_) {
      return;
    }
    
    if (PlayerController::IsPlayer(collided_entity)) {
      PlayerController* pc = PlayerController::Get();
      if (contact_normal.y > 0.58f) {
        pc->SetEnemyBounce();
        contact->SetEnabled(false);
        stopm_ = true;
      }
    }

    if (std::abs(contact_normal.y) < 0.1f) {
      // Change the direction of turtle. No need for Goomba
      auto& tc = entity->GetComponent<TransformComponent>();
      tc.UpdateScale(X, going_right_ ? -1.0f : 1.0f);

      going_right_ = contact_normal.x < 0.0f;
    }
  }
  
  void GoombaController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
  }
  
  void GoombaController::Update(Timestep ts) {
    // If Fixture need to be reset
    if (rbc_->reset_fixture_) {
      const auto& cc = entity_.GetComponent<CircleColliiderComponent>();
      Scene::ResetCircleColliderFixture(entity_.GetComponent<TransformComponent>(), rbc_, cc);
      rbc_->reset_fixture_ = false;
    }
    
    // Destory the entity if enemy is dead after stomp
    if (is_dead_) {
      time_to_kill_ -= ts;
      rbc_->SetVelocity({0., 0});
      if (time_to_kill_ <= 0) {
        entity_.scene_->DestroyEntity(entity_);
      }
      return;
    }
    
    EnemyController::Update(ts, &entity_, rbc_);
  }
  
  void GoombaController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    EnemyController::PreSolve(collided_entity, contact, contact_normal, &entity_);
    if (stopm_) {
      is_dead_ = true;
      
      rbc_->SetVelocity({0, 0});
      rbc_->SetAngularVelocity(0.0f);
      rbc_->is_sensor = true;
      rbc_->reset_fixture_ = true;
      
      auto& qc = entity_.GetComponent<QuadComponent>();
      qc.sprite.sprite_images = SpriteManager::GetEnemySprite(EnemyType::Goomba, EnemyState::Dying);
      
      const auto& tc = entity_.GetComponent<TransformComponent>();
      RuntimeItemManager::Spawn(Items::Score, entity_.scene_, {tc.Position().x, tc.Position().y + 1}, score::EnemyKill);
      PlayerController::Get()->AddScore(score::EnemyKill);
    }
  }
  
  void GoombaController::Copy(void *script) {
    if (!script) return;
    GoombaController* enemy_script = reinterpret_cast<GoombaController*>(script);
    IK_ASSERT(enemy_script);
    
    is_dead_ = enemy_script->is_dead_;
    stopm_ = enemy_script->stopm_;
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    
    height_ = enemy_script->height_;
    
    time_to_kill_ = enemy_script->time_to_kill_;

    acceleration_ = enemy_script->acceleration_;
    velocity_ = enemy_script->velocity_;
  }
  
  void DuckController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
    
    auto& tc = entity_.GetComponent<TransformComponent>();
    tc.UpdateScale(X, going_right_ ? -1.0f : 1.0f);
    height_ = 2.0f;
  }
  
  void DuckController::Update(Timestep ts) {
    EnemyController::Update(ts, &entity_, rbc_);
  }
  
  void DuckController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    EnemyController::PreSolve(collided_entity, contact, contact_normal, &entity_);
  }
  
  void DuckController::Copy(void *script) {
    if (!script) return;
    DuckController* enemy_script = reinterpret_cast<DuckController*>(script);
    IK_ASSERT(enemy_script);
    
    is_dead_ = enemy_script->is_dead_;
    stopm_ = enemy_script->stopm_;
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    
    height_ = enemy_script->height_;
    
    acceleration_ = enemy_script->acceleration_;
    velocity_ = enemy_script->velocity_;
  }

  struct EnemyScriptData {
    std::unordered_map<EnemyType, EnemyData> script_map;
  };
  static std::shared_ptr<EnemyScriptData> data;
  
  void EnemyManager::Init() {
    data = std::make_shared<EnemyScriptData>();
    
    static auto goomba_load_fn = ScriptLoader(mario::GoombaController);
    static auto duck_load_fn = ScriptLoader(mario::DuckController);
    
    data->script_map[EnemyType::Goomba] = {EnemyType::Goomba, goomba_load_fn, "mario::GoombaController"};
    data->script_map[EnemyType::Duck] = {EnemyType::Duck, duck_load_fn, "mario::DuckController"};
  }
  void EnemyManager::Shutdown() {
    data.reset();
  }

  const EnemyData& EnemyManager::GetData(EnemyType type) {
    return data->script_map.at(type);
  }

  bool EnemyManager::IsEnemy(const std::string& tag) {
    if(data->script_map.find(GetType(tag)) == data->script_map.end())
      return false;
    return true;
  }
  
  EnemyType EnemyManager::GetType(const std::string& tag) {
    if (tag == "Goomba") return EnemyType::Goomba;
    if (tag == "Duck") return EnemyType::Duck;
    return EnemyType::None;
  }
  
  NativeScriptComponent* EnemyManager::AddScript(Entity* entity, EnemyType type) {
    const EnemyData& data = GetData(type);
    switch (type) {
      case EnemyType::Goomba: return MarioPrefab::AddScript<mario::GoombaController>(entity, data.script_name, data.loader_fun);
      case EnemyType::Duck: return MarioPrefab::AddScript<mario::DuckController>(entity, data.script_name, data.loader_fun);
      default: return nullptr;
        break;
    }
  }

} // namespace mario
