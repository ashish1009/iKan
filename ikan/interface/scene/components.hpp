//
//  components.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

namespace ikan {
  
  template<typename... Component>
  struct ComponentGroup {
  };
  
#define ALL_COPY_COMPONENTS

  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
  // Stores the components that need to be copied
  using AllCopyComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
} // namespace ikan
