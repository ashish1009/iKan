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
      
      // Change the direction of Velocity
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
    
    // If hit by bullet then kill the enemy
    if (collided_entity->HasComponent<BulletComponent>()) {
      Die(entity);
    }
    
    // Check Player hit for stomp
    if (PlayerController::IsPlayer(collided_entity)) {
      PlayerController* pc = PlayerController::Get();
      if (contact_normal.y > 0.58f) {
        pc->SetEnemyBounce();
        contact->SetEnabled(false);
        stopm_ = true;
      }
    }

    // Change direction on hitting obstacle
    if (std::abs(contact_normal.y) < 0.1f) {
      // Change the direction of turtle. No need for Goomba
      auto& tc = entity->GetComponent<TransformComponent>();
      tc.UpdateScale(X, going_right_ ? -1.0f : 1.0f);

      going_right_ = contact_normal.x < 0.0f;
    }
  }
  
  void EnemyController::Die(Entity* entity) {
    auto& tc = entity->GetComponent<TransformComponent>();
    tc.UpdateScale(Y, tc.Scale().y * -1.0f);
    
    auto& rbc = entity->GetComponent<RigidBodyComponent>();
    rbc.is_sensor = true;
    reset_fixture_ = true;
    
    die_animation_ = true;
    is_dead_ = true;
  }
  
  void EnemyController::RenderGui() {
    ImGui::Text(" On Ground         | %s", on_ground_ ? "True" : "False");
    ImGui::Text(" Stomp             | %s", stopm_ ? "True" : "False");
    ImGui::Text(" Is Dead           | %s", is_dead_ ? "True" : "False");
    ImGui::Text(" Is Dying          | %s", is_dying_ ? "True" : "False");
    
    ImGui::Text(" Acceleration      | %f : %f ", acceleration_.x, acceleration_.y);
    ImGui::Text(" Velocity          | %f : %f ", velocity_.x, velocity_.y);
    ImGui::Text(" Terminal Velocity | %f : %f ", terminal_velocity_.x, terminal_velocity_.y);
  }
  
  void GoombaController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
  }
  
  void GoombaController::Update(Timestep ts) {
    // If Fixture need to be reset
    if (reset_fixture_) {
      const auto& cc = entity_.GetComponent<CircleColliiderComponent>();
      Scene::ResetCircleColliderFixture(entity_.GetComponent<TransformComponent>(), rbc_, cc);
      reset_fixture_ = false;
    }
    
    // Destory the entity if enemy is dead after stomp
    if (is_dying_) {
      time_to_kill_ -= ts;
      rbc_->SetVelocity({0., 0});
      if (time_to_kill_ <= 0) {
        entity_.scene_->DestroyEntity(entity_);
        is_dead_ = true;
      }
      return;
    }
    
    // Play Die Animation for Goomba
    if (die_animation_) {
      die_animation_time_ -= ts;
      if (die_animation_time_ > 0.8f) {
        rbc_->ApplyForceToCenter({5.0f, 20.0f});
      }
      else if (die_animation_time_ <= 0.8f and die_animation_time_ > 0.0f) {
        rbc_->ApplyForceToCenter({5.0f, -20.0f});
      }
      else if (die_animation_time_ <= 0.0f) {
        entity_.scene_->DestroyEntity(entity_);
        die_animation_ = false;
      }
      return;
    }
    
    EnemyController::Update(ts, &entity_, rbc_);
  }
  
  void GoombaController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    EnemyController::PreSolve(collided_entity, contact, contact_normal, &entity_);
    if (stopm_) {
      is_dying_ = true;
      
      rbc_->SetVelocity({0, 0});
      rbc_->SetAngularVelocity(0.0f);
      rbc_->is_sensor = true;
      reset_fixture_ = true;
      
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
    is_dying_ = enemy_script->is_dying_;
    stopm_ = enemy_script->stopm_;
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    die_animation_ = enemy_script->die_animation_;
    reset_fixture_ = enemy_script->reset_fixture_;

    height_ = enemy_script->height_;
    die_animation_time_ = enemy_script->die_animation_time_;

    acceleration_ = enemy_script->acceleration_;
    velocity_ = enemy_script->velocity_;
    walk_speed_ = enemy_script->walk_speed_;
    
    time_to_kill_ = enemy_script->time_to_kill_;
  }
  
  void GoombaController::RenderGui() {
    EnemyController::RenderGui();
  }
  
  void TurtleController::Create(Entity entity) {
    entity_ = entity;
    rbc_ = MarioPrefab::AddRigidBody(&entity_, RigidBodyComponent::RbBodyType::Dynamic);
    
    Initialize(&entity_, rbc_);
    
    auto& tc = entity_.GetComponent<TransformComponent>();
    tc.UpdateScale(X, going_right_ ? -1.0f : 1.0f);
    
    if (!is_dying_)
      height_ = 2.0f;
  }
  
  void TurtleController::Update(Timestep ts) {
    if (reset_fixture_) {
      const auto& pbc = entity_.GetComponent<PillBoxColliderComponent>();
      Scene::ResetPillBoxColliderFixture(entity_.GetComponent<TransformComponent>(), rbc_, pbc);
      reset_fixture_ = false;
      
      if (is_dead_) {
        entity_.GetComponent<TransformComponent>().UpdateScale(Y, 1.0f);
        entity_.GetComponent<QuadComponent>().sprite.sprite_images = SpriteManager::GetEnemySprite(EnemyType::Turtle, EnemyState::Dying);
      }
    }
    
    if (die_animation_) {
      die_animation_time_ -= ts;
      if (die_animation_time_ > 0.8f) {
        rbc_->ApplyForceToCenter({5.0f, 50.0f});
      }
      else if (die_animation_time_ <= 0.8f and die_animation_time_ > 0.0f) {
        rbc_->ApplyForceToCenter({5.0f, -50.0f});
      }
      else if (die_animation_time_ <= 0.0f) {
        entity_.scene_->DestroyEntity(entity_);
        die_animation_ = false;
      }
      return;
    }

    if (is_dying_ and !force_applied_) {
      time_to_revive_ -= ts;

      auto& qc = entity_.GetComponent<QuadComponent>();
      if (time_to_revive_ > 0.0f and time_to_revive_ <= 1.0f ) {
        qc.sprite.sprite_images = SpriteManager::GetEnemySprite(EnemyType::Turtle, EnemyState::Revive);
      }
      else if (time_to_revive_ <= 0.0f) {
        height_ = 2.0f;
        entity_.GetComponent<TransformComponent>().UpdateScale(Y, height_);

        auto& pbc = entity_.GetComponent<PillBoxColliderComponent>();
        pbc.height = 0.8f;
        pbc.offset.y = -0.20f;
        pbc.RecalculateColliders();

        qc.sprite.sprite_images = SpriteManager::GetEnemySprite(EnemyType::Turtle, EnemyState::Alive);

        // Add Impulse to push e out of ground while changing size
        rbc_->ApplyImpulseToCenter({0, 1.0});

        SetAppliedForce(false);
        is_dying_ = false;
        reset_fixture_ = true;
      }
    }
    
    EnemyController::Update(ts, &entity_, rbc_);
  }
  
  void TurtleController::PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {
    EnemyController::PreSolve(collided_entity, contact, contact_normal, &entity_);
    
    if (is_dying_) {
      if (PlayerController::IsPlayer(collided_entity)) {
        if (std::abs(contact_normal.x) > 0.8f and std::abs(contact_normal.y) < 0.4f) {
          SetAppliedForce(true);
        }
      }
    }

    if (stopm_) {
      stopm_ = false;
      time_to_revive_ = time_to_revive_limit_;
      
      // After Forece apply kindly reset the walk speed to 0
      SetAppliedForce(false);
      walk_speed_ = 0.0f;
      
      height_ = 1.0f;
      entity_.GetComponent<TransformComponent>().UpdateScale(Y, height_);
      
      auto& pbc = entity_.GetComponent<PillBoxColliderComponent>();
      pbc.height = 0.5f;
      pbc.offset.y = 0.0f;
      pbc.RecalculateColliders();
      
      auto& qc = entity_.GetComponent<QuadComponent>();
      qc.sprite.sprite_images = SpriteManager::GetEnemySprite(EnemyType::Turtle, EnemyState::Dying);

      reset_fixture_ = true;

      // Add Score only of Turtle is alive
      if (!is_dying_) {
        is_dying_ = true;

        const auto& tc = entity_.GetComponent<TransformComponent>();
        RuntimeItemManager::Spawn(Items::Score, entity_.scene_, {tc.Position().x, tc.Position().y + 1}, score::EnemyKill);
        PlayerController::Get()->AddScore(score::EnemyKill);
      }
    }
  }
  
  void TurtleController::SetAppliedForce(bool force) {
    force_applied_ = force;
    if (force) {
      walk_speed_ = 8.0f;
      if (!entity_.HasComponent<BulletComponent>())
        entity_.AddComponent<BulletComponent>();
    }
    else {
      walk_speed_ = 4.0f;
      if (entity_.HasComponent<BulletComponent>())
        entity_.RemoveComponent<BulletComponent>();
    }
  }
  
  void TurtleController::Copy(void *script) {
    if (!script) return;
    TurtleController* enemy_script = reinterpret_cast<TurtleController*>(script);
    IK_ASSERT(enemy_script);
    
    is_dead_ = enemy_script->is_dead_;
    is_dying_ = enemy_script->is_dying_;
    stopm_ = enemy_script->stopm_;
    going_right_ = enemy_script->going_right_;
    on_ground_ = enemy_script->on_ground_;
    die_animation_ = enemy_script->die_animation_;
    reset_fixture_ = enemy_script->reset_fixture_;
    
    height_ = enemy_script->height_;
    die_animation_time_ = enemy_script->die_animation_time_;

    acceleration_ = enemy_script->acceleration_;
    velocity_ = enemy_script->velocity_;
    walk_speed_ = enemy_script->walk_speed_;
    
    time_to_revive_ = enemy_script->time_to_revive_;
    force_applied_ = enemy_script->force_applied_;
  }
  void TurtleController::RenderGui() {
    EnemyController::RenderGui();
    ImGui::Text(" Force applied     | %s", force_applied_ ? "True" : "False");
    ImGui::Text(" Time to Revive    | %f", time_to_revive_);

  }

  struct EnemyScriptData {
    std::unordered_map<EnemyType, EnemyData> script_map;
  };
  static std::shared_ptr<EnemyScriptData> data;
  
  void EnemyManager::Init() {
    data = std::make_shared<EnemyScriptData>();
    
    static auto goomba_load_fn = ScriptLoader(mario::GoombaController);
    static auto turtle_load_fn = ScriptLoader(mario::TurtleController);
    
    data->script_map[EnemyType::Goomba] = {EnemyType::Goomba, goomba_load_fn, "mario::GoombaController"};
    data->script_map[EnemyType::Turtle] = {EnemyType::Turtle, turtle_load_fn, "mario::TurtleController"};
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
    if (tag == "Turtle") return EnemyType::Turtle;
    return EnemyType::None;
  }
  
  NativeScriptComponent* EnemyManager::AddScript(Entity* entity, EnemyType type) {
    const EnemyData& data = GetData(type);
    switch (type) {
      case EnemyType::Goomba: return MarioPrefab::AddScript<mario::GoombaController>(entity, data.script_name, data.loader_fun);
      case EnemyType::Turtle: return MarioPrefab::AddScript<mario::TurtleController>(entity, data.script_name, data.loader_fun);
      default: return nullptr;
        break;
    }
  }

} // namespace mario
