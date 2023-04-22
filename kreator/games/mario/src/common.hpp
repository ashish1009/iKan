//
//  common.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  class MarioPrefab {
  public:
    static void AddQuadComponent(Entity* entity);
    MAKE_PURE_STATIC(MarioPrefab);
  };
  
} // namespace
