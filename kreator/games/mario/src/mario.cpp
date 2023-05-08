//
//  mario.cpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#include "mario.hpp"
#include "sprite_manager.hpp"
#include "player.hpp"
#include "block_controller.hpp"
#include "runtime_items.hpp"
#include "enemy.hpp"

namespace mario {
    
  Mario::Mario() {
    MARIO_LOG("Creating Mario Game Data ... ");
    Batch2DRenderer::AddQuadData(2000);
    SpriteManager::Init();
    BlockScriptManager::Init();
    RuntimeItemManager::Init();
    EnemyManager::Init();
  }
  
  Mario::~Mario() {
    MARIO_LOG("Destroying Mario Game Data ... ");
    SpriteManager::Shutdown();
    BlockScriptManager::Shutdown();
    RuntimeItemManager::Shutdown();
    EnemyManager::Shutdown();
  }
  
  void Mario::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    MARIO_LOG("Initialising Mario Game Data ... ");
    
    scene_ = scene;
    viewport_ = viewport;
    timer_ = 0;
    time_left_ = MaxTime;
    
    SearchOrCreatePlayer();
    AddScriptsToEntities();
  }
  
  void Mario::Update(Timestep ts) {
    if (is_playing_) {
      // Timer
      timer_ += ts;
      time_left_ = MaxTime - (uint32_t)timer_;
    }
    
    PlayerController* pc = PlayerController::Get();

    // Score and All text
    TextRenderer::BeginBatch(FixedCamera::projection);
    
    text_data_.Render("MARIO", 0, 0);
    
    // Get the score
    static constexpr uint32_t score_string_limit = 6;
    std::string score = "";
    std::string player_score = std::to_string(pc->GetScore());
    uint32_t num_zero_add = score_string_limit - (uint32_t)player_score.size();
    score = std::string(num_zero_add, '0') + player_score;

    text_data_.Render(score, 1, 0);
    
    Batch2DRenderer::BeginBatch(FixedCamera::projection);
    static std::vector<SubTextureRef> coin_subtex = SpriteManager::GetItemSprite(Items::AnimatedCoin);
    static int32_t anim_idx = 0;
    float speed = 20.0f;
    if (anim_idx >= speed * coin_subtex.size() or anim_idx < 1)
      anim_idx = 0;
    
    Batch2DRenderer::DrawQuad(Math::GetTransformMatrix({text_data_.col_pos[1] - 10, text_data_.row_pos[1] + 8, 0}, {0, 0, 0}, {22, 22, 1}),
                              coin_subtex.at(anim_idx / speed), {1, 1, 1, 1});
    anim_idx++;
    Batch2DRenderer::EndBatch();
    
    // Get the coins
    static constexpr uint32_t coin_string_limit = 2;
    std::string coins = "";
    std::string player_coins = std::to_string(pc->GetCoins());
    num_zero_add = coin_string_limit - (uint32_t)player_coins.size();
    coins = std::string(num_zero_add, '0') + player_coins;

    text_data_.Render(" x " + coins, 1, 1);
    
    text_data_.Render("WORLD", 0, 2);
    text_data_.Render(std::to_string(world_) + " - " + std::to_string(level_), 1, 2);
    
    text_data_.Render("TIME", 0, 3);
    text_data_.Render(std::to_string(time_left_), 1, 3);
    
    TextRenderer::EndBatch();
  }
  
  void Mario::SearchOrCreatePlayer() {
    const std::string player_name = "Mario Player";
    bool found_player = false;
    Entity player_entity;

    // Search for Player in current scene
    MARIO_LOG("Searching Mario Player ");
    auto tag_view = scene_->GetEntitesWith<TagComponent>();
    for (auto entity : tag_view) {
      const auto& player_tag = tag_view.get<TagComponent>(entity).tag;
      // - Note: This has to be the payer Name in the game
      if (player_tag == player_name) {
        found_player = true;
        player_entity = Entity(entity, scene_.get());
        MARIO_LOG("Found Mario Player ... ");
        break;
      }
    }
    
    // Create new player if not found
    if (!found_player) {
      MARIO_LOG("Mario Player didnt found. Creating New ... ");
      player_entity = scene_->CreateEntity(player_name);
    }
    
    // Add Essentials Components
    MarioPrefab::AddQuad(&player_entity, SM::GetTexture(SpriteType::Player),
                         SM::GetPlayerSprite(PlayerState::Small, PlayerAction::Idle, PlayerAction::Idle));
    MarioPrefab::AddRigidBody(&player_entity, RigidBodyComponent::RbBodyType::Dynamic);
    MarioPrefab::AddPillBoxCollider(&player_entity, {0.4f, 0.5f});
    MarioPrefab::AddScript<mario::PlayerController>(&player_entity, "mario::PlayerController", ScriptLoader(mario::PlayerController));
  }
  
  void Mario::AddScriptsToEntities() {
    auto tag_view = scene_->GetEntitesWith<TagComponent>();
    
    for (auto e : tag_view) {
      const auto &c = tag_view.get<TagComponent>(e);
      if (IsBlock(c.tag)) {
        Entity entity = Entity(e, scene_.get());
        MarioPrefab::AddScript<BlockController>(&entity, "mario::BlockController", BSM::GetLoaderFn(c.tag), BSM::GetType(c.tag), BSM::GetCount(c.tag));
      }
      else if (EnemyManager::IsEnemy(c.tag)) {
        Entity entity = Entity(e, scene_.get());
        EnemyManager::AddScript(&entity, EnemyManager::GetType(c.tag));
      }

    } // For each Tag
    
    MARIO_LOG("Added Script in each Mario Entities");
  }
  
  void Mario::SetViewportSize(uint32_t width, uint32_t height) {
    text_data_.SetViewport(width, height);
  }

} // namespace mario
