//
//  pipeline.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

#include "renderer/utils/renderer.hpp"

namespace ikan {
  
  /// Interface class for Storing Renderer Pipeline. Implementation is depending on the supported Renerer API.
  class Pipeline {
  public:
    /// This static function creates Ref type instance of Pipeline. Instance depending on the current Renderer API
    static std::shared_ptr<Pipeline> Create();

    virtual ~Pipeline() noexcept = default;
  };
  
} // namespace ikan
