//
//  components.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

namespace ikan {
  
  struct IDComponent {
    UUID id = 0;
    IDComponent(const UUID& id);
    ~IDComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(IDComponent);
  };
  
  struct TagComponent {
    std::string tag = "Default Entity";
    TagComponent(const std::string& tag);
    ~TagComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(TagComponent);
  };
  
  template<typename... Component>
  struct ComponentGroup {
  };
  
#define ALL_COPY_COMPONENTS IDComponent, TagComponent

  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
  // Stores the components that need to be copied
  using AllCopyComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
} // namespace ikan
