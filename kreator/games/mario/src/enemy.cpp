//
//  enemy.cpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#include "enemy.hpp"
#include "common.hpp"

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
    CheckOnGround(entity);
    
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
  
  void GoombaController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
  }
  void GoombaController::Update(Timestep ts) {
    EnemyController::Update(ts, &entity_, rbc_);
  }
  void GoombaController::Copy(void *script) {
    if (!script) return;
    GoombaController* enemy_script = reinterpret_cast<GoombaController*>(script);
    IK_ASSERT(enemy_script);
    
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    
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
  void DuckController::Copy(void *script) {
    if (!script) return;
    DuckController* enemy_script = reinterpret_cast<DuckController*>(script);
    IK_ASSERT(enemy_script);
    
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    
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
