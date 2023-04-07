//
//  renderer_api.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

namespace ikan {
    
  class Pipeline;

  /// This class is the interface for Renderer APIs
  class RendererAPI {
  public:
    enum class GlFunc {
      Always, Never, Less, Equal, LEqual, Greater, LGreater, NotEqual
    };

    /// This function creates the instance of renderer API based on the supported API
    static std::unique_ptr<RendererAPI> Create();

    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
    
    /// This function changes the bit value for depth bit
    /// - Parameter state: new depth state
    virtual void Depth(bool state) const = 0;
    /// This functin changes the bit value for Blend bit
    /// - Parameter state: new Blend state
    virtual void Blend(bool state) const = 0;
    /// This function changes the bit value of multisample
    /// - Parameter state: new Multi sample state
    virtual void MultiSample(bool state) const = 0;
    /// This function change depth function
    /// - Parameter func: depth function type
    virtual void DepthFunc(GlFunc func) const = 0;
    
    /// This function begins the wireframe renderer
    virtual void BeginWireframe() const = 0;
    /// This function ends the wireframe renderer
    virtual void EndWireframe() const = 0;
    
    /// This function clear all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    virtual void SetClearColor(const glm::vec4& color) const = 0;
    /// This function clear all the bits of renderer screen
    virtual void ClearBits() const = 0;
    /// This function Clears Depth Bit
    virtual void ClearDepthBit() const = 0;
    /// This function Clears Color Biut
    virtual void ClearColorBit() const = 0;
    /// This function Clears Stensil Bit
    virtual void ClearStencilBit() const = 0;
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    virtual void SetViewport(uint32_t width, uint32_t height) const = 0;
    
    /// This funciton returns the Pixel ID from Frame buffer
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixel_data: pixel value
    ///   - pixel_id_index: pixel index
    virtual void GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixel_id_index, int32_t& pixel_data) const = 0;
    
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count = 0) const = 0;
    /// This API draws Lines Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - vertex_count: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawLines(const std::shared_ptr<Pipeline>& pipeline, uint32_t vertex_count) const = 0;
    /// This API draws Indexed Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawArrays(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) const = 0;
  };
  
} // namespace ikan
