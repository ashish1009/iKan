//
//  components.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "components.hpp"

namespace ikan {
  
#define ComponentLog 1
  
#if ComponentLog
  #define COMP_LOG(...)       IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
  #define COMP_COPY_LOG(...)  IK_CORE_TRACE(LogModule::Component, __VA_ARGS__);
#else
  #define COMP_LOG(...)
  #define COMP_COPY_LOG(...)
#endif
  
  // -------------------------------------------------------------------------
  // ID Component
  // -------------------------------------------------------------------------
  IDComponent::IDComponent(const UUID& id) : id(id) { COMP_LOG("Creating ID Component"); }
  IDComponent::~IDComponent() { COMP_LOG("Destroying ID Component"); }
  IDComponent::IDComponent(const IDComponent& other) : id(other.id) { COMP_COPY_LOG("Copying ID Component"); }
  IDComponent::IDComponent(IDComponent&& other) : id(other.id) { COMP_COPY_LOG("Moving ID Component"); }
  IDComponent& IDComponent::operator=(const IDComponent& other) {
    COMP_COPY_LOG("Copying with = operator ID Component");
    id = other.id;
    return *this;
  }
  IDComponent& IDComponent::operator=(IDComponent&& other) {
    COMP_COPY_LOG("Moving with = operator ID Component");
    id = other.id;
    return *this;
  }
  
  // -------------------------------------------------------------------------
  // Tag Component
  // -------------------------------------------------------------------------
  TagComponent::TagComponent(const std::string& tag) : tag(tag) { COMP_LOG("Creating Tag Component"); }
  TagComponent::~TagComponent() { COMP_LOG("Destroying Tag Component"); }
  TagComponent::TagComponent(const TagComponent& other) : tag(other.tag) { COMP_COPY_LOG("Copying Tag Component"); }
  TagComponent::TagComponent(TagComponent&& other) : tag(other.tag) { COMP_COPY_LOG("Moving Tag Component"); }
  TagComponent& TagComponent::operator=(const TagComponent& other) {
    COMP_COPY_LOG("Copying with = operator Tag Component");
    tag = other.tag;
    return *this;
  }
  TagComponent& TagComponent::operator=(TagComponent&& other) {
    COMP_COPY_LOG("Moving with = operator Tag Component");
    tag = other.tag;
    return *this;
  }
  
} // namespace ikan
