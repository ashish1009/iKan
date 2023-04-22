//
//  entity.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "core_entity.hpp"
#include "scene/components.hpp"
#include "scene/scene_serialiser.hpp"

namespace ikan {
    
  Entity::Entity(entt::entity handle, Scene* scene)
  : entity_handle_(handle), scene_(scene) {
    IK_CORE_TRACE(LogModule::Entity, "Creating Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity::Entity(Entity&& other) : entity_handle_(other.entity_handle_),
  scene_(other.scene_) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Moving Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity::Entity(const Entity& other) : entity_handle_(other.entity_handle_),
  scene_(other.scene_) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Copying Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity& Entity::operator=(const Entity& other) {
    entity_handle_ = other.entity_handle_;
    scene_ = other.scene_;
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Copying Entity using = operator with Handle {0} ...", (uint32_t)entity_handle_);
    return *this;
  }
  Entity& Entity::operator=(Entity&& other) {
    entity_handle_ = other.entity_handle_;
    scene_ = other.scene_;
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Moving Entity using = operator with Handle {0} ...", (uint32_t)entity_handle_);
    return *this;
  }
  Entity::~Entity() {
    IK_CORE_TRACE(LogModule::Entity, "Destroying Entity with Handle: {0} !!!", (uint32_t)entity_handle_);
  }

  bool Entity::IsValidScene() const {
    return scene_ and StringUtils::GetExtensionFromFilePath(scene_->GetFilePath()) == saved_scene_extension_;
  }

  UUID Entity::GetUUID() const { return GetComponent<IDComponent>().id; }
  const std::string& Entity::GetName() const { return GetComponent<TagComponent>().tag; }

  bool Entity::IsValid(Entity* e) { return e and e->scene_->IsEntityPresentInMap((entt::entity)(*e)); }

} // namespace ikan
