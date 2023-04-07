//
//  renderer.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

#include "renderer/utils/renderer_api.hpp"

namespace ikan {
  
  class Shader;
  class Texture;
  
  /// All renderer ID type
  using RendererID = uint32_t;

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
    
    /// This funciton Clears all the Renderer Bits and set the Clear color to new color
    /// - Parameter color: New color of renderer Clear color
    static void Clear(const glm::vec4& color);
    /// This function Sets the Clear color to new color
    /// - Parameter color: New color of renderer Clear color
    static void SetClearColor(const glm::vec4& color);
    /// This function Clears all the Renderer Bits
    static void ClearBits();
    /// This function Clears Depth Bit
    static void ClearDepthBit();
    /// This function Clears Color Biut
    static void ClearColorBit();
    /// This function Clears Stensil Bit
    static void ClearStencilBit();
    
    /// This funciton Enables or disables Depth Field
    /// - Parameter state: flag to be set as Depth
    static void Depth(bool state);
    /// This funciton Enables or disables Blend Field
    /// - Parameter state: flag to be set as Blend
    static void Blend(bool state);
    /// This funciton Enables or disables MultiSample Field
    /// - Parameter state: flag to be set as MultiSample
    static void MultiSample(bool state);
    
    /// This function change depth function
    /// - Parameter func: depth function type
    static void DepthFunc(RendererAPI::GlFunc func);
    
    /// This function begins the wireframe renderer
    static void BeginWireframe();
    /// This function ends the wireframe renderer
    static void EndWireframe();
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    static void SetViewport(uint32_t width, uint32_t height);
    
    /// This function returns the the Pixel ID from Viewport
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixel_id_index: pixell index in fb
    ///   - pixeld_data: piixel value
    static void GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixel_id_index, int32_t& pixeld_data);

    /// This function resets the renderer Stats each frame
    /// - Note: Only those stats will be reset that need to be reset each frame
    static void ResetStatsEachFrame();
    /// This function resets all the renderer stats
    static void ResetStats();
    /// This function renderers Imgui to show renderer stats
    /// - Parameter is_open: flag to show or hide the widget
    static void RenderStatsGui(bool *is_open = nullptr);
    /// This function renderers Imgui to show renderer stats
    /// - Parameter is_open: flag to show or hide the widget
    static void Render2DStatsGui(bool *is_open = nullptr);

    /// This function returns the shader pointer from the library. If not present then  create new shader and store in the library
    /// - Parameter path: path of shader
    [[nodiscard]] static std::shared_ptr<Shader> GetShader(const std::string& path);
    /// This function returns the shader pointer from the library. If not present then  create new shader and store in the library
    /// - Parameters:
    ///   - path: path of textre
    ///   - linear: min linear flag
    [[nodiscard]] static std::shared_ptr<Texture> GetTexture(const std::string& path, bool linear = true);

    MAKE_PURE_STATIC(Renderer);
  };
  
  namespace renderer_utils {
    
#ifdef IK_DEBUG_FEATURE
    /// This function returns the Renderer API name in string
    /// - Parameter api: renderer API in enum format
    std::string GetRendererApiName(Renderer::Api api);
#endif
    
  } // namespace renderer_utils

} // namespace ikan
