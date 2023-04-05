//
//  batch_2d_renderer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "batch_2d_renderer.hpp"

namespace ikan {
  
  /// This structure holds the common batch renderer data for Quads, circle and lines
  struct CommonBatchData {
    /// Max element to be rendered in single batch
    uint32_t max_element = 0;
    uint32_t max_vertices = 0;
    
    friend struct Shape2DCommonData;
    friend struct LineData;

  private:
    CommonBatchData() = default;
  };

  /// This structure holds the common batch renderer data for Quads and circle
  struct Shape2DCommonData : CommonBatchData {
    // Constants
    static constexpr uint32_t VertexForSingleElement = 4;
    static constexpr uint32_t IndicesForSingleElement = 6;
    
    /// Store the Vertex and Indices size
    uint32_t max_indices = 0;
    
    void SetMaxElements(uint32_t max_elements) {
      max_element = max_elements;
      max_vertices = max_elements * VertexForSingleElement;
      max_indices = max_elements * IndicesForSingleElement;
    }
  };
  
  /// Batch Data to Rendering Quads
  struct QuadData : Shape2DCommonData {
  };
  static std::unique_ptr<QuadData> quad_data_;
  
  /// Batch Data to Rendering Circles
  struct CircleData : Shape2DCommonData {
  };
  static std::unique_ptr<CircleData> circle_data_;
  
  /// Batch Data to Rendering Lines
  struct LineData : CommonBatchData {
    static constexpr uint32_t VertexForSingleLine = 2;

    void SetMaxElements(uint32_t max_elements) {
      max_element = max_elements;
      max_vertices = max_elements * VertexForSingleLine;
    }
  };
  static std::unique_ptr<LineData> line_data_;
  
  void Batch2DRenderer::Initialise(uint32_t max_quads, uint32_t max_cirlces, uint32_t max_lines) {
    IK_CORE_TRACE(LogModule::Batch2DRenderer, "Initialising the Batch Renderer 2D ...");
    AddQuadData(max_quads);
    AddCircleData(max_cirlces);
    AddLineData(max_lines);
  }
  
  void Batch2DRenderer::Shutdown() {
    IK_CORE_TRACE(LogModule::Batch2DRenderer, "Shutting Down the Batch Renderer 2D !!!");
  }
  
  void Batch2DRenderer::AddQuadData(uint32_t max_quads) {
    RETURN_IF(max_quads == 0);
    
    // We will append the max quad data. so we store the previous one
    if (quad_data_) {
      max_quads += quad_data_->max_element;
      
      // If we already allocated memory then delete the older one first.
      quad_data_.reset();
    }
  
    // Allocate memory for Quad Data
    quad_data_ = std::make_unique<QuadData>();
    
    // Set all max element, vertices and indices
    quad_data_->SetMaxElements(max_quads);
  }
  
  void Batch2DRenderer::AddCircleData(uint32_t max_circles) {
    RETURN_IF(max_circles == 0);
    
    // We will append the max quad data. so we store the previous one
    if (circle_data_) {
      max_circles += circle_data_->max_element;
      
      // If we already allocated memory then delete the older one first.
      circle_data_.reset();
    }
    
    // Allocate memory for Circle Data
    circle_data_ = std::make_unique<CircleData>();
    
    // Set all max element, vertices and indices
    circle_data_->SetMaxElements(max_circles);
  }
  
  void Batch2DRenderer::AddLineData(uint32_t max_lines) {
    RETURN_IF(max_lines == 0);

    // We will append the max quad data. so we store the previous one
    if (line_data_) {
      max_lines += line_data_->max_element;
      
      // If we already allocated memory then delete the older one first.
      line_data_.reset();
    }
    
    // Allocate memory for Line Data
    line_data_ = std::make_unique<LineData>();

    // Set all max element, vertices and indices
    line_data_->SetMaxElements(max_lines);
  }
  
} // namespace ikan
