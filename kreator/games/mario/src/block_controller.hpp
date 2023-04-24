//
//  block_controller.hpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class BlockType {
    Empty, Coin, PowerUp, Star
  };
 
  class PlayerController;
  
  class BlockController : public ScriptableEntity {
  public:
    BlockController(BlockType type, uint32_t count);
    ~BlockController() = default;
    
    void Create(Entity entity) override;
    void Update(Timestep ts) override;
    void RenderGui() override;
    void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) override;
    void Copy(void* script) override;
    
  private:
    /// This function action the Player Hit
    /// - Parameter pc: player controller
    void PlayerHit(PlayerController* pc);

    static constexpr float speed_ = 3.0f;

    bool going_up_ = true;
    bool animation_ = false;
    bool active_ = true;

    glm::vec2 start_pos_;
    glm::vec2 end_pos_;

    BlockType type_ = BlockType::Empty;
    uint32_t count_ = 0;
  };
  
  class BlockScriptManager {
  public:
    /// This function Initialse the Block Scripts
    static void Init();
    /// This function Destorys the Block Scripts
    static void Shutdown();
    
    /// This function returns the script loader function for a specific Block
    /// - Parameter tag: type name of Block
    static ScriptLoaderFn GetLoaderFn(const std::string& tag);
    /// This function returns the type of Block from name
    /// - Parameter tag: type name of Block
    static BlockType GetType(const std::string& tag);
    /// This function returns the count of Item present in block
    /// - Parameter tag: type name of Block
    static uint32_t GetCount(const std::string& tag);
  };
  using BSM = BlockScriptManager;
  
  inline bool IsBlock(const std::string& tag) {
    return tag == "Brick" or tag == "CoinBonus" or tag == "MultiCoinBonus" or tag == "PowerUpBonus" or "Start";
  }
  
} // namespace mario
