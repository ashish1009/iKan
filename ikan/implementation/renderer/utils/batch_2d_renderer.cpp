//
//  batch_2d_renderer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "batch_2d_renderer.hpp"
#include "renderer/graphics/pipeline.hpp"

namespace ikan {
  
  /// This structure holds the common batch renderer data for Quads, circle and lines
  struct CommonBatchData {
    /// Max element to be rendered in single batch
    uint32_t max_element = 0;
    uint32_t max_vertices = 0;
    
    std::shared_ptr<Pipeline> pipeline;
    
    friend struct Shape2DCommonData;
    friend struct LineData;

  private:
    CommonBatchData() = default;
  };

  /// This structure holds the common batch renderer data for Quads and circle
  struct Shape2DCommonData : CommonBatchData {
    /// Single vertex for a Quad or Circle
    struct CommonVertex {
      glm::vec3 position;
      glm::vec4 color;
      glm::vec2 texture_coords;
      
      float texture_index;
      float tiling_factor;
    };

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
    
    virtual ~Shape2DCommonData() = default;
  };
  
  /// Batch Data to Rendering Quads
  struct QuadData : Shape2DCommonData {
    /// Single vertex of a Quad
    struct Vertex : CommonVertex {
      int32_t pixel_id;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertex_buffer_base_ptr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertex_buffer_ptr = nullptr;
    
    virtual ~QuadData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
    }
  };
  static std::unique_ptr<QuadData> quad_data_;
  
  /// Batch Data to Rendering Circles
  struct CircleData : Shape2DCommonData {
    /// Single vertex of a Circle
    struct Vertex : CommonVertex {
      glm::vec3 local_position;
      float thickness;
      float fade;
      
      int32_t pixel_id;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertex_buffer_base_ptr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertex_buffer_ptr = nullptr;
    
    virtual ~CircleData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
    }
  };
  static std::unique_ptr<CircleData> circle_data_;
  
  /// Batch Data to Rendering Lines
  struct LineData : CommonBatchData {
    static constexpr uint32_t VertexForSingleLine = 2;

    /// Single vertex of a Circle
    struct Vertex {
      glm::vec3 position;       // Position of a Quad
      glm::vec4 color;          // Color of a Quad
    };

    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertex_buffer_base_ptr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertex_buffer_ptr = nullptr;

    virtual ~LineData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
    }

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
    std::unique_ptr<QuadData>& data = quad_data_;
    
    // If data have already created then append the data to previous one else create new memory
    if (data) {
      max_quads += data->max_element;
    }
    else {
      data = std::make_unique<QuadData>();
    }
    
    // Set the max element, max vertices and max indices
    data->SetMaxElements(max_quads);
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertex_buffer_base_ptr = new QuadData::Vertex[data->max_vertices];
    
    // Create Pipeline instance
    data->pipeline = Pipeline::Create();
  }
  
  void Batch2DRenderer::AddCircleData(uint32_t max_circles) {
    RETURN_IF(max_circles == 0);
    
    // If data have already created then append the data to previous one else create new memory
    if (circle_data_) {
      max_circles += circle_data_->max_element;
    }
    else {
      circle_data_ = std::make_unique<CircleData>();
    }
    
    // Set the max element, max vertices and max indices
    circle_data_->SetMaxElements(max_circles);
    
    // Allocating the memory for vertex Buffer Pointer
    circle_data_->vertex_buffer_base_ptr = new CircleData::Vertex[circle_data_->max_vertices];
    
    // Create Pipeline instance
    quad_data_->pipeline = Pipeline::Create();
  }
  
  void Batch2DRenderer::AddLineData(uint32_t max_lines) {
    RETURN_IF(max_lines == 0);

    // If data have already created then append the data to previous one else create new memory
    if (line_data_) {
      max_lines += line_data_->max_element;
    }
    else {
      line_data_ = std::make_unique<LineData>();
    }

    // Set the max element and max vertices
    line_data_->SetMaxElements(max_lines);
    
    // Allocating the memory for vertex Buffer Pointer
    line_data_->vertex_buffer_base_ptr = new LineData::Vertex[line_data_->max_vertices];
    
    // Create Pipeline instance
    quad_data_->pipeline = Pipeline::Create();
  }
  
} // namespace ikan
