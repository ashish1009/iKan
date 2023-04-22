//
//  common.hpp
//  kreator
//
//  Created by Ashish . on 22/04/23.
//

#pragma once

#define TextureRef std::shared_ptr<Texture>
#define SubTextureRef std::shared_ptr<SubTexture>

namespace mario {
  
  using namespace ikan;
  
  class MarioPrefab {
  public:
    static void AddQuadComponent(Entity* entity, TextureRef tex, const std::vector<SubTextureRef>& sprites);
    MAKE_PURE_STATIC(MarioPrefab);
  };
  
} // namespace
