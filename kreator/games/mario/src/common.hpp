//
//  common.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#define TextureRef std::shared_ptr<Texture>
#define SubTextureRef std::shared_ptr<SubTexture>
#define MARIO_LOG(...) IK_TRACE("Mario", __VA_ARGS__);

namespace mario {
  
  using namespace ikan;

  static const uint32_t NoScore = 0;
  static const uint32_t CoinScore = 100;

  class MarioPrefab {
  public:
    /// This function add the quad component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: enity
    ///   - tex: texture to be added
    ///   - sprites: sprites vecotr
    static QuadComponent* AddQuad(Entity* entity, TextureRef tex, const std::vector<SubTextureRef>& sprites);
    /// This function add the Ridig Body component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - type: type of body
    static RigidBodyComponent* AddRigidBody(Entity* entity, RigidBodyComponent::RbBodyType type);
    /// This function add the Circle Collider component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - radius: radius of circle
    ///   - offset: offset of circle
    static CircleColliiderComponent* AddCircleCollider(Entity* entity, float radius = 0.5f, const glm::vec2& offset = glm::vec2(0.0f));
    /// This function add the Pill Box Collider component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - size: size of pill
    ///   - offset: offset of pill
    static PillBoxColliderComponent* AddPillBoxCollider(Entity* entity, const glm::vec2& size = glm::vec2(0.5f), const glm::vec2& offset = glm::vec2(0.0f));
    /// This function add the Text component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - text: Text to be rendererd
    ///   - color: Color of text
    static TextComponent* AddText(Entity* entity, const std::string& text, const glm::vec4& color = glm::vec4(1.0f));

    template<typename... Args>
    /// This function add the Native script component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - scrip_name: Script name
    ///   - fn: scrip loader fnction
    ///   - args: Arguments required for Bind function
    /// - Note: There should not be Sctipt compomnent present in tne entity
    static NativeScriptComponent* AddScript(Entity* entity, const std::string& scrip_name, const ScriptLoaderFn& fn, Args&&... args) {
      NativeScriptComponent* nsc = &(entity->AddComponent<NativeScriptComponent>(scrip_name, fn));
      return nsc;
    }
    
    template<typename T, typename... Args>
    /// This function add the Native script component if not present else add the required parameter
    /// - Parameters:
    ///   - entity: entity
    ///   - scrip_name: Script name
    ///   - fn: scrip loader fnction
    ///   - args: Arguments required for Bind function
    static NativeScriptComponent* AddScript(Entity* entity, const std::string& scrip_name, const ScriptLoaderFn& fn, Args&&... args) {
      NativeScriptComponent* nsc;
      if (!entity->HasComponent<NativeScriptComponent>()) {
        nsc = &(entity->AddComponent<NativeScriptComponent>(scrip_name, fn));
      }
      else {
        nsc = &(entity->GetComponent<NativeScriptComponent>());
        nsc->loader_function = fn;
      }
      nsc->Bind<T>(std::forward<Args>(args)...);
      return nsc;
    }
    MAKE_PURE_STATIC(MarioPrefab);
  };
  
} // namespace
