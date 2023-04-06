//
//  batch_2d_renderer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "batch_2d_renderer.hpp"
#include "renderer/graphics/pipeline.hpp"
#include "renderer/graphics/renderer_buffer.hpp"

namespace ikan {
  
  /// This structure holds the common batch renderer data for Quads, circle and lines
  struct CommonBatchData {
    /// Max element to be rendered in single batch
    uint32_t max_element = 0;
    uint32_t max_vertices = 0;
    
    std::shared_ptr<Pipeline> pipeline;
    std::shared_ptr<VertexBuffer> vertex_buffer;

    void CommonInit(uint32_t max_elem, uint32_t max_vertices_single_elem) {
      max_element = max_elem;
      max_vertices = max_elem * max_vertices_single_elem;
      
      // Create Pipeline instance
      pipeline = Pipeline::Create();
    }
    
    virtual ~CommonBatchData() {
      pipeline.reset();
    }
    
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
    
    void Initialise(uint32_t max_elements) {
      CommonInit(max_elements, VertexForSingleElement);
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

    void Initialise(uint32_t max_elements) {
      CommonInit(max_elements, VertexForSingleLine);
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
    
    quad_data_.reset();
    circle_data_.reset();
    line_data_.reset();
  }
  
  void Batch2DRenderer::AddQuadData(uint32_t max_element) {
    RETURN_IF(max_element == 0);
    std::unique_ptr<QuadData>& data = quad_data_;
    
    // If data have already created then append the data to previous one
    if (data) {
      max_element += data->max_element;
      data.reset();
    }
  
    // Allocate memory for Quad Data
    data = std::make_unique<QuadData>();
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertex_buffer_base_ptr = new QuadData::Vertex[data->max_vertices];
    
    // Initialize the data for Common shape
    data->Initialise(max_element);
    
    // Create vertes Buffer
    data->vertex_buffer = VertexBuffer::Create(data->max_vertices * sizeof(QuadData::Vertex));
    data->vertex_buffer->AddLayout({
      { "a_Position",     ShaderDataType::Float3 },
      { "a_Color",        ShaderDataType::Float4 },
      { "a_TexCoords",    ShaderDataType::Float2 },
      { "a_TexIndex",     ShaderDataType::Float },
      { "a_TilingFactor", ShaderDataType::Float },
      { "a_ObjectID",     ShaderDataType::Int },
    });
    data->pipeline->AddVertexBuffer(data->vertex_buffer);
    
    // Create Index Buffer
    uint32_t* quad_indices = new uint32_t[data->max_indices];
    uint32_t offset = 0;
    for (size_t i = 0; i < data->max_indices; i += Shape2DCommonData::IndicesForSingleElement) {
      quad_indices[i + 0] = offset + 0;
      quad_indices[i + 1] = offset + 1;
      quad_indices[i + 2] = offset + 2;
      
      quad_indices[i + 3] = offset + 2;
      quad_indices[i + 4] = offset + 3;
      quad_indices[i + 5] = offset + 0;
      
      offset += 4;
    }
    
    // Create Index Buffer in GPU for storing Indices
    std::shared_ptr<IndexBuffer> ib = IndexBuffer::CreateWithCount(quad_indices, data->max_indices);
    data->pipeline->SetIndexBuffer(ib);
    delete[] quad_indices;
  }
  
  void Batch2DRenderer::AddCircleData(uint32_t max_element) {
    RETURN_IF(max_element == 0);
    std::unique_ptr<CircleData>& data = circle_data_;
    
    // If data have already created then append the data to previous one
    if (data) {
      max_element += data->max_element;
      data.reset();
    }

    // Allocate memory for Circle Data
    data = std::make_unique<CircleData>();
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertex_buffer_base_ptr = new CircleData::Vertex[data->max_vertices];
    
    // Initialize the data for Common shape
    data->Initialise(max_element);
    
    // Create vertes Buffer
    data->vertex_buffer = VertexBuffer::Create(data->max_vertices * sizeof(CircleData::Vertex));
    data->vertex_buffer->AddLayout({
      { "a_Position",     ShaderDataType::Float3 },
      { "a_Color",        ShaderDataType::Float4 },
      { "a_TexCoords",    ShaderDataType::Float2 },
      { "a_TexIndex",     ShaderDataType::Float },
      { "a_TilingFactor", ShaderDataType::Float },
      { "a_LocalPosition",ShaderDataType::Float3 },
      { "a_Thickness",    ShaderDataType::Float },
      { "a_Fade",         ShaderDataType::Float },
      { "a_ObjectID",     ShaderDataType::Int },
    });
    data->pipeline->AddVertexBuffer(data->vertex_buffer);

    // Create Index Buffer
    uint32_t* quad_indices = new uint32_t[data->max_indices];
    uint32_t offset = 0;
    for (size_t i = 0; i < data->max_indices; i += Shape2DCommonData::IndicesForSingleElement) {
      quad_indices[i + 0] = offset + 0;
      quad_indices[i + 1] = offset + 1;
      quad_indices[i + 2] = offset + 2;
      
      quad_indices[i + 3] = offset + 2;
      quad_indices[i + 4] = offset + 3;
      quad_indices[i + 5] = offset + 0;
      
      offset += 4;
    }
    
    // Create Index Buffer in GPU for storing Indices
    std::shared_ptr<IndexBuffer> ib = IndexBuffer::CreateWithCount(quad_indices, data->max_indices);
    data->pipeline->SetIndexBuffer(ib);
    delete[] quad_indices;
  }
  
  void Batch2DRenderer::AddLineData(uint32_t max_element) {
    RETURN_IF(max_element == 0);
    std::unique_ptr<LineData>& data = line_data_;
    
    // If data have already created then append the data to previous one
    if (data) {
      max_element += data->max_element;
      data.reset();
    }

    // Allocate memory for Line Data
    data = std::make_unique<LineData>();

    // Allocating the memory for vertex Buffer Pointer
    data->vertex_buffer_base_ptr = new LineData::Vertex[data->max_vertices];

    // Initialize the data for Common shape
    data->Initialise(max_element);
    
    // Create vertes Buffer
    data->vertex_buffer = VertexBuffer::Create(data->max_vertices * sizeof(LineData::Vertex));
    data->vertex_buffer->AddLayout({
      { "a_Position",     ShaderDataType::Float3 },
      { "a_Color",        ShaderDataType::Float4 },
    });
    data->pipeline->AddVertexBuffer(data->vertex_buffer);
  }
  
} // namespace ikan
