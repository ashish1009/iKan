//
//  enemy.hpp
//  kreator
//
//  Created by Ashish . on 05/05/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class EnemyType {
    None, Goomba, Duck
  };
  
  enum class EnemyState {
    Alive, Dying, Dead
  };
  
  class EnemyController {
  public:
    /// This function initialise the Enemy common data
    /// - Parameters:
    ///   - rbc: rigid body of enemy
    ///   - entity: entity of enemy
    void Initialize(Entity* entity, RigidBodyComponent* rbc);
    
  protected:
    static constexpr float free_fall_factor = 1.0f;
    
    bool going_right_ = true;
    float height_ = 2.0f;
    glm::vec2 acceleration_;
  };
  
  class GoombaController : public ScriptableEntity, EnemyController {
  public:
    void Create(Entity entity) override;
  };

  class DuckController : public ScriptableEntity, EnemyController {
  public:
    void Create(Entity entity) override;
  };
  
  struct EnemyData {
    EnemyType type;
    ScriptLoaderFn loader_fun;
    std::string script_name;
    EnemyData() = default;
    EnemyData(EnemyType type, ScriptLoaderFn fun, std::string script_name) : type(type), loader_fun(fun), script_name(script_name) { }
  };

  class EnemyManager {
  public:
    /// This funciton initialise the Enemy Scripts
    static void Init();
    /// This function shuts destroy the Enemy scripts
    static void Shutdown();
    /// This function returns the enemy data from type
    static const EnemyData& GetData(EnemyType type);
    /// This function retuens is the tag is enemy
    /// - Parameter tag: entity tag
    static bool IsEnemy(const std::string& tag);
    /// This function returns the type of enemy from string
    /// - Parameter tag: entity tag
    static EnemyType GetType(const std::string& tag);
    /// This function add the Native script component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - type: type of enemy
    static NativeScriptComponent* AddScript(Entity* entity, EnemyType type);
  };


} // namespace mario
