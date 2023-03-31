//
//  renderer.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

namespace ikan {
  
  class Renderer {
  public:
    /// Supported APIs in ikan engine
    enum class Api : uint8_t {
      None, OpenGl
      // Add other supported API with time
    };

    // -------------
    // Fundamentals
    // -------------
    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer API as 'api'.
    static void Initialize(Api api);
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for any Graphics API.
    static void Shutdown();
    
    MAKE_PURE_STATIC(Renderer);
  };
  
} // namespace ikan
