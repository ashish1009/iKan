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
    Alive, Dying, Dead, Revive
  };
  
  class EnemyController {
  protected:
    /// This function initialise the Enemy common data
    /// - Parameters:
    ///   - rbc: rigid body of enemy
    ///   - entity: entity of enemy
    void Initialize(Entity* entity, RigidBodyComponent* rbc);
    /// This function check the entity on ground
    /// - Parameter entity: entity of player
    void CheckOnGround(Entity* entity);
    /// This function updates the Common data for enemy
    /// - Parameters:
    ///   - entity: entity of enemy
    ///   - ts: time step
    ///   - rbc: rigid body pointer
    void Update(Timestep ts, Entity* entity, RigidBodyComponent* rbc);
    /// This function solve the collider for enemy
    /// - Parameters:
    ///   - collided_entity: collided entity
    ///   - contact: contact point
    ///   - contact_normal: contact normal
    ///   - entity: entity of enemy
    void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal, Entity* entity);
    /// This function renders the data in Gui
    void RenderGui();
    
    static constexpr float free_fall_factor = 1.0f;
    static constexpr glm::vec2 terminal_velocity_ = {8.1f, 18.1f};
    static constexpr float walk_speed_ = 4.0f;

    bool is_dead_ = false;
    bool is_dying_ = false;
    bool on_ground_ = false;
    bool going_right_ = true;
    bool stopm_ = false;
    
    float height_ = 1.0f;
    
    glm::vec2 acceleration_;
    glm::vec2 velocity_;
  };
  
  class GoombaController : public ScriptableEntity, EnemyController {
  public:
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    void RenderGui() override;
    
  private:
    float time_to_kill_ = 0.5f;
  };

  class DuckController : public ScriptableEntity, EnemyController {
  public:
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    void RenderGui() override;
    
  private:
    bool force_applied_ = false;
    float time_to_revive_ = 2.5f;
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
