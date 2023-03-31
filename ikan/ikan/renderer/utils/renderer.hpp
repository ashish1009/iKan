//
//  renderer.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

namespace ikan {
  
  /// This class is the wrapper class to interact with all the module that will be needed to render any object or control any renderer API
  ///   - Create instance of renderer API
  ///   - Initialize any user defined rednerer
  ///   - Contain API to be used by Core or client
  ///   - Shut down or destroy all the renderer data that were created by Renderer
  ///   - Manage the shader and texture library
  class Renderer {
  public:
    /// Supported APIs in ikan engine
    enum class Api : uint8_t {
      None, OpenGl
      // Add other supported API with time
    };

    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer API as 'api'.
    static void Initialize(Api api);
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for any Graphics API.
    static void Shutdown();
    
    /// This function returns the current API supported
    static Api GetApi();

    MAKE_PURE_STATIC(Renderer);
  };
  
} // namespace ikan
