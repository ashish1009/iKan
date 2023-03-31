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
  ///   - Note: APIs to be called in following order while initialising
  ///     1. CreateRendererDate(Api)
  ///     <Create Window with Renderer Context>
  ///     2. Initialize()
  ///     <Run the Game Loop>
  ///     <Use all Renderer Runtime APIs>
  ///     3. Shutdown();
  class Renderer {
  public:
    /// Supported APIs in ikan engine
    enum class Api : uint8_t {
      None, OpenGl
      // Add other supported API with time
    };

    /// Stores the capability of renderer
    struct Capabilities {
      std::string vendor;
      std::string renderer;
      std::string version;
      
      void Log();
      static Capabilities& Get();
      
    private:
      ~Capabilities() = default;
      Capabilities() = default;
      
      DELETE_COPY_MOVE_CONSTRUCTORS(Capabilities);
    };

    /// This function Creates memory for rendere Data.
    /// - Note: To be called before initializing any Open GL or GLFW instances
    /// - Parameter api: current renderer API
    static void CreateRendererData(Api api);
    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer API as 'api'.
    /// - Note: To be called after creating Open GL Rendere Context (should be created while creating GLFW window)
    static void Initialize();
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for any Graphics API.
    static void Shutdown();
    
    /// This function returns the current API supported
    static Api GetApi();

    MAKE_PURE_STATIC(Renderer);
  };
  
} // namespace ikan
