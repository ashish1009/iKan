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
  
  void GoombaController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
  }
  
  void DuckController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
    
    auto& tc = entity_.GetComponent<TransformComponent>();
    tc.UpdateScale(X, going_right_ ? -1.0f : 1.0f);
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
