//
//  renderer_api.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

namespace ikan {
    
  /// This class is the interface for Renderer APIs
  class RendererAPI {
  public:
    /// This function creates the instance of renderer API based on the supported API
    static std::unique_ptr<RendererAPI> Create();

    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
  };
  
} // namespace ikan
