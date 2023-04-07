//
//  batch_2d_renderer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "batch_2d_renderer.hpp"
#include "renderer/utils/renderer_stats.hpp"
#include "renderer/graphics/pipeline.hpp"
#include "renderer/graphics/renderer_buffer.hpp"
#include "renderer/graphics/shader.hpp"
#include "renderer/graphics/texture.hpp"

namespace ikan {

#define BATCH_INFO(...) IK_CORE_INFO(LogModule::Batch2DRenderer, __VA_ARGS__)
#define BATCH_TRACE(...) IK_CORE_TRACE(LogModule::Batch2DRenderer, __VA_ARGS__)

  static constexpr glm::vec2 texture_coords_[] = {
    { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
  };

  /// This structure holds the common batch renderer data for Quads, circle and lines
  struct CommonBatchData {
    /// Max element to be rendered in single batch
    uint32_t max_element = 0;
    uint32_t max_vertices = 0;
    
    std::shared_ptr<Pipeline> pipeline;
    std::shared_ptr<VertexBuffer> vertex_buffer;
    std::shared_ptr<Shader> shader;
    
    std::function<void(std::shared_ptr<Shader>,
                       const glm::mat4&)> load_cam_to_shader = [](std::shared_ptr<Shader> shader,
                                                                  const glm::mat4& cam_view_proj_mat) {
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", cam_view_proj_mat);
      shader->Unbind();
    };

    void CommonInit(uint32_t max_elem, uint32_t max_vertices_single_elem) {
      max_element = max_elem;
      max_vertices = max_elem * max_vertices_single_elem;
      
      // Create Pipeline instance
      pipeline = Pipeline::Create();
    }
    
    virtual ~CommonBatchData() {
      pipeline.reset();
      vertex_buffer.reset();
      shader.reset();
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
    /// Count of Indices to be renderer in Single Batch
    uint32_t index_count = 0;

    /// Stores all the 16 Texture in array so that there is no need to load texture each frame
    /// NOTE: Load only if new texture is added or older replaced with new
    std::array<std::shared_ptr<Texture>, MaxTextureSlotsInShader> texture_slots;
    
    /// Texture Slot index sent to Shader to render a specific Texture from slots
    /// Slot 0 is reserved for white texture (No Image only color)
    uint32_t texture_slot_index = 1; // 0 = white texture

    /// Basic vertex of quad (Vertex of circle is taken as Quad only)
    glm::vec4 vertex_base_position[4];

    void Initialise(uint32_t max_elements) {
      CommonInit(max_elements, VertexForSingleElement);
      max_indices = max_elements * IndicesForSingleElement;
      
      // Creating white texture for colorful quads witout any texture or sprite
      if (!texture_slots[0]) {
        uint32_t whiteTextureData = 0xffffffff;
        texture_slots[0] = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
      }
      
      // Create Index Buffer
      uint32_t* indices = new uint32_t[max_indices];
      uint32_t offset = 0;
      for (size_t i = 0; i < max_indices; i += Shape2DCommonData::IndicesForSingleElement) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        
        offset += 4;
      }
      
      // Create Index Buffer in GPU for storing Indices
      std::shared_ptr<IndexBuffer> ib = IndexBuffer::CreateWithCount(indices, max_indices);
      pipeline->SetIndexBuffer(ib);
      delete[] indices;
      
      // Setting basic Vertex point of quad
      vertex_base_position[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
      vertex_base_position[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
      vertex_base_position[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
      vertex_base_position[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }
    
    void StartCommonBatch(const glm::mat4& cam_view_proj_mat) {
      load_cam_to_shader(shader, cam_view_proj_mat);
      StartCommonBatch();
    }

    void StartCommonBatch() {
      index_count = 0;
      texture_slot_index = 1;
    }
    
    void Flush() {
      shader->Bind();
      
      for (uint32_t i = 0; i < texture_slot_index; i++)
        texture_slots[i]->Bind(i);
      
      // Render the Scene
      Renderer::DrawIndexed(pipeline, index_count);
    }

    virtual ~Shape2DCommonData() {
      for(auto texture : texture_slots)
        texture.reset();
    }
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
    
    void StartBatch(const glm::mat4& cam_view_proj_mat) {
      StartCommonBatch(cam_view_proj_mat);
      StartBatch();
    }
    void StartBatch() {
      vertex_buffer_ptr = vertex_buffer_base_ptr;
    }

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
    
    void StartBatch(const glm::mat4& cam_view_proj_mat) {
      StartCommonBatch(cam_view_proj_mat);
      StartBatch();
    }

    void StartBatch() {
      vertex_buffer_ptr = vertex_buffer_base_ptr;
    }

    virtual ~CircleData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
    }
  };
  static std::unique_ptr<CircleData> circle_data_;
  
  /// Batch Data to Rendering Lines
  struct LineData : CommonBatchData {
    static constexpr uint32_t VertexForSingleLine = 2;

    /// Count of Indices to be renderer in Single Batch
    uint32_t vertex_count = 0;

    /// Single vertex of a Circle
    struct Vertex {
      glm::vec3 position;       // Position of a Quad
      glm::vec4 color;          // Color of a Quad
    };

    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertex_buffer_base_ptr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertex_buffer_ptr = nullptr;

    void StartBatch(const glm::mat4& cam_view_proj_mat) {
      load_cam_to_shader(shader, cam_view_proj_mat);
      StartBatch();
    }
    void StartBatch() {
      vertex_count = 0;
      vertex_buffer_ptr = vertex_buffer_base_ptr;
    }

    void Initialise(uint32_t max_elements) {
      CommonInit(max_elements, VertexForSingleLine);
    }

    virtual ~LineData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
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
        
    if (quad_data_) {
      BATCH_TRACE("Destroying the Batch Renderer Quad Data");
      BATCH_TRACE("  Max Quads per Batch            {0}", quad_data_->max_element);
      BATCH_TRACE("  Max Texture Slots per Batch    {0}", MaxTextureSlotsInShader);
      BATCH_TRACE("  Vertex Buffer used             {0} B", quad_data_->max_vertices * sizeof(QuadData::Vertex));
      BATCH_TRACE("  Index Buffer used              {0} B", quad_data_->max_indices * sizeof(uint32_t));
      BATCH_TRACE("  Shader Used                    {0}", quad_data_->shader->GetName());
      quad_data_.reset();
    }
    
    if (circle_data_) {
      BATCH_TRACE("Destroying the Batch Renderer Circle Data");
      BATCH_TRACE("  Max Circles per Batch          {0}", circle_data_->max_element);
      BATCH_TRACE("  Max Texture Slots per Batch    {0}", MaxTextureSlotsInShader);
      BATCH_TRACE("  Vertex Buffer used             {0}", circle_data_->max_vertices * sizeof(CircleData::Vertex));
      BATCH_TRACE("  Vertex Buffer used             {0}", circle_data_->max_indices * sizeof(uint32_t));
      BATCH_TRACE("  Shader used                    {0}", circle_data_->shader->GetName());
      circle_data_.reset();
    }
    if (line_data_) {
      BATCH_TRACE("Destroying the Batch Renderer Line Data");
      BATCH_TRACE("  Max Lines per Batch            {0}", line_data_->max_element);
      BATCH_TRACE("  Vertex Buffer used             {0} B", line_data_->max_vertices * sizeof(LineData::Vertex));
      BATCH_TRACE("  Shader used                    {0}", line_data_->shader->GetName());
      line_data_.reset();
    }
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
        
    // Setup the Quad Shader
    data->shader = Renderer::GetShader(DM::CoreAsset("shaders/batch_quad_shader.glsl"));
    
    RendererStatistics::Get().stats_2d_.max_quads = data->max_element;
    
    BATCH_TRACE("Initialized Batch Renderer for Quad Data");
    BATCH_TRACE("  Max Quads per Batch              {0}", data->max_element);
    BATCH_TRACE("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
    BATCH_TRACE("  Vertex Buffer used               {0} B", data->max_vertices * sizeof(QuadData::Vertex));
    BATCH_TRACE("  Index Buffer used                {0} B", data->max_indices * sizeof(uint32_t));
    BATCH_TRACE("  Shader Used                      {0}", data->shader->GetName());
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
    
    // Setup the Quad Shader
    data->shader = Renderer::GetShader(DM::CoreAsset("shaders/batch_circle_shader.glsl"));
    
    RendererStatistics::Get().stats_2d_.max_circles = data->max_element;
    
    BATCH_TRACE("Initialized Batch Renderer for Circle Data");
    BATCH_TRACE("  Max Circles per Batch            {0}", data->max_element);
    BATCH_TRACE("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
    BATCH_TRACE("  Vertex Buffer used               {0} B", data->max_vertices * sizeof(CircleData::Vertex));
    BATCH_TRACE("  Index Buffer used                {0} B", data->max_indices * sizeof(uint32_t));
    BATCH_TRACE("  Shader Used                      {0}", data->shader->GetName());
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
    
    // Setup the Quad Shader
    data->shader = Renderer::GetShader(DM::CoreAsset("shaders/batch_line_shader.glsl"));
    
    RendererStatistics::Get().stats_2d_.max_lines = data->max_element;
    
    BATCH_TRACE("Initialized Batch Renderer for Line Data");
    BATCH_TRACE("  Max Lines per Batch              {0}", data->max_element);
    BATCH_TRACE("  Vertex Buffer used               {0} B", data->max_vertices * sizeof(LineData::Vertex));
    BATCH_TRACE("  Shader Used                      {0}", data->shader->GetName());
  }
  
  void Batch2DRenderer::LogData() {
    BATCH_INFO("    Batch Renderer Data ...");

    BATCH_INFO("        Per Batch");

    BATCH_INFO("            Max Texture slots             | {0}", MaxTextureSlotsInShader);
    BATCH_INFO("            Max Quads                     | {0}", quad_data_->max_element);
    BATCH_INFO("            Max Circles                   | {0}", circle_data_->max_element);
    BATCH_INFO("            Max Lines                     | {0}", line_data_->max_element);
    
    BATCH_INFO("        GPU Memory");
    [[maybe_unused]] uint32_t vertex_buffer_size = quad_data_->max_vertices * sizeof(QuadData::Vertex);
    vertex_buffer_size += circle_data_->max_vertices * sizeof(CircleData::Vertex);
    vertex_buffer_size += line_data_->max_vertices * sizeof(LineData::Vertex);
    BATCH_INFO("            Vertex Buffer used            | {0} Bytes", vertex_buffer_size);

    [[maybe_unused]] uint32_t index_buffer_size = (quad_data_->max_indices + circle_data_->max_indices) * sizeof(uint32_t);
    BATCH_INFO("            Index Buffer used             | {0} Bytes", index_buffer_size);
    
    BATCH_INFO("        Shaders");
    BATCH_INFO("            Quad                          | {0}", quad_data_->shader->GetName());
    BATCH_INFO("            Circle                        | {0}", circle_data_->shader->GetName());
    BATCH_INFO("            Line                          | {0}", line_data_->shader->GetName());
  }

  void Batch2DRenderer::BeginBatch(const glm::mat4& cam_view_proj_mat) {
    if (quad_data_) quad_data_->StartBatch(cam_view_proj_mat);
    if (circle_data_) circle_data_->StartBatch(cam_view_proj_mat);
    if (line_data_) line_data_->StartBatch(cam_view_proj_mat);
  }
  
  void Batch2DRenderer::EndBatch() {
    Flush();
  }

  void Batch2DRenderer::NextBatch() {
    if (quad_data_) quad_data_->StartBatch();
    if (circle_data_) circle_data_->StartBatch();
    if (line_data_) line_data_->StartBatch();
  }
  
  void Batch2DRenderer::Flush() {
    if (quad_data_ and quad_data_->index_count) {
      uint32_t data_size = (uint32_t)((uint8_t*)quad_data_->vertex_buffer_ptr - (uint8_t*)quad_data_->vertex_buffer_base_ptr);
      quad_data_->vertex_buffer->SetData(quad_data_->vertex_buffer_base_ptr, data_size);
      quad_data_->Flush();
    }
    
    if (circle_data_ and circle_data_->index_count) {
      uint32_t dataSize = (uint32_t)((uint8_t*)circle_data_->vertex_buffer_ptr - (uint8_t*)circle_data_->vertex_buffer_base_ptr);
      circle_data_->vertex_buffer->SetData(circle_data_->vertex_buffer_base_ptr, dataSize);
      circle_data_->Flush();
    }
    
    if (line_data_ and line_data_->vertex_count) {
      uint32_t dataSize = (uint32_t)((uint8_t*)line_data_->vertex_buffer_ptr - (uint8_t*)line_data_->vertex_buffer_base_ptr);
      line_data_->vertex_buffer->SetData(line_data_->vertex_buffer_base_ptr, dataSize);
      
      line_data_->shader->Bind();      
      // Render the Scene
      Renderer::DrawLines(line_data_->pipeline, line_data_->vertex_count);
    }
  }
  
  void Batch2DRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t object_id) {
    DrawTextureQuad(transform, nullptr, texture_coords_, 1.0f /* tiling factor */, color, object_id);
  }
  
//  void Batch2DRenderer::DrawQuad(const glm::vec3& position, const glm::vec3& radius, const glm::vec3& rotation, const glm::vec4& color,
//                                 float thickness = 1.0f, float fade = (float)0.005, int32_t object_id = -1) {
//    DrawTextureQuad(transform, nullptr, texture_coords_, 1.0f /* tiling factor */, color, object_id);
//  }
  
  void Batch2DRenderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint_color,
                                 float tiling_factor, int32_t object_id) {
    DrawTextureQuad(transform, texture ? texture : nullptr, texture_coords_, tiling_factor, tint_color, object_id );
  }
  
  
  void Batch2DRenderer::DrawTextureQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture,
                                        const glm::vec2* texture_coords, float tiling_factor, const glm::vec4& tint_color, int32_t object_id) {
    // If number of indices increase in batch then start new batch
    if (quad_data_->index_count >= quad_data_->max_indices) {
      BATCH_INFO("Starts the new batch as number of indices ({0}) increases in the previous batch", quad_data_->index_count);
      EndBatch();
      quad_data_->StartBatch();
    }
    
    float texture_index = 0.0f;
    if (texture) {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < quad_data_->texture_slot_index; i++) {
        if (quad_data_->texture_slots[i].get() == texture.get()) {
          // Found the current textue in the batch
          texture_index = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (texture_index == 0.0f) {
        // If number of slots increases max then start new batch
        if (quad_data_->texture_slot_index >= MaxTextureSlotsInShader) {
          BATCH_INFO("Starts the new batch as number of texture slot ({0}) increases in the previous batch", quad_data_->texture_slot_index);
          NextBatch();
        }
        
        // Loading the current texture in the first free slot slot
        texture_index = (float)quad_data_->texture_slot_index;
        quad_data_->texture_slots[quad_data_->texture_slot_index] = texture;
        quad_data_->texture_slot_index++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++) {
      quad_data_->vertex_buffer_ptr->position         = transform * quad_data_->vertex_base_position[i];
      quad_data_->vertex_buffer_ptr->color            = tint_color;
      quad_data_->vertex_buffer_ptr->texture_coords   = texture_coords[i];
      quad_data_->vertex_buffer_ptr->texture_index    = texture_index;
      quad_data_->vertex_buffer_ptr->tiling_factor    = tiling_factor;
      quad_data_->vertex_buffer_ptr->pixel_id         = object_id;
      quad_data_->vertex_buffer_ptr++;
    }
    
    quad_data_->index_count += Shape2DCommonData::IndicesForSingleElement;
    
    RendererStatistics::Get().index_count += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertex_count += Shape2DCommonData::VertexForSingleElement;
    
    RendererStatistics::Get().stats_2d_.quads ++;
  }
  
  void Batch2DRenderer::DrawCircle(const glm::vec3& position, const glm::vec3& radius, const glm::vec3& rotation, const glm::vec4& color,
                                   float thickness, float fade, int32_t object_id) {
    auto transform  = Math::GetTransformMatrix(position, rotation, radius);
    DrawTextureCircle(transform, nullptr, 1.0f /* tiling factor */, color, thickness, fade, object_id);
  }
  
  
  void Batch2DRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int32_t object_id) {
    DrawTextureCircle(transform, nullptr, 1.0f /* tiling factor */, color, thickness, fade, object_id);
  }
  void Batch2DRenderer::DrawCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint_color,
                                   float tiling_factor, float thickness, float fade, int32_t object_id){
    DrawTextureCircle(transform, texture, tiling_factor, tint_color, thickness, fade, object_id);
  }
  
  void Batch2DRenderer::DrawTextureCircle(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, float tiling_factor,
                                          const glm::vec4& tint_color, float thickness, float fade, int32_t object_id) {
    // If number of indices increase in batch then start new batch
    if (circle_data_->index_count >= circle_data_->max_indices) {
      BATCH_INFO("Starts the new batch as number of indices ({0}) increases in the previous batch", circle_data_->index_count);
      EndBatch();
      circle_data_->StartBatch();
    }
    
    float texture_index = 0.0f;
    if (texture) {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < circle_data_->texture_slot_index; i++) {
        if (circle_data_->texture_slots[i].get() == texture.get()) {
          // Found the current textue in the batch
          texture_index = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (texture_index == 0.0f) {
        // If number of slots increases max then start new batch
        if (circle_data_->texture_slot_index >= MaxTextureSlotsInShader) {
          BATCH_INFO("Starts the new batch as number of texture slot ({0}) increases in the previous batch", circle_data_->texture_slot_index);
          NextBatch();
        }
        
        // Loading the current texture in the first free slot slot
        texture_index = (float)circle_data_->texture_slot_index;
        circle_data_->texture_slots[circle_data_->texture_slot_index] = texture;
        circle_data_->texture_slot_index++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++) {
      circle_data_->vertex_buffer_ptr->position         = transform * circle_data_->vertex_base_position[i];
      circle_data_->vertex_buffer_ptr->color            = tint_color;
      circle_data_->vertex_buffer_ptr->texture_coords   = 2.0f * circle_data_->vertex_base_position[i];
      circle_data_->vertex_buffer_ptr->texture_index    = texture_index;
      circle_data_->vertex_buffer_ptr->tiling_factor    = tiling_factor;
      circle_data_->vertex_buffer_ptr->local_position   = 2.0f * circle_data_->vertex_base_position[i];
      circle_data_->vertex_buffer_ptr->thickness        = thickness;
      circle_data_->vertex_buffer_ptr->fade             = fade;
      circle_data_->vertex_buffer_ptr->pixel_id         = object_id;
      circle_data_->vertex_buffer_ptr++;
    }
    
    circle_data_->index_count += Shape2DCommonData::IndicesForSingleElement;
    
    RendererStatistics::Get().index_count += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertex_count += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get().stats_2d_.circles++;
  }
  
  void Batch2DRenderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color) {
    
    // If number of indices increase in batch then start new batch
    if (line_data_->vertex_count >= line_data_->max_vertices) {
      BATCH_INFO("Starts the new batch as number of vertices ({0}) increases in the previous batch", line_data_->vertex_count);
      EndBatch();
      line_data_->StartBatch();
    }
    
    line_data_->vertex_buffer_ptr->position = p0;
    line_data_->vertex_buffer_ptr->color = color;
    line_data_->vertex_buffer_ptr++;
    
    line_data_->vertex_buffer_ptr->position = p1;
    line_data_->vertex_buffer_ptr->color = color;
    line_data_->vertex_buffer_ptr++;
    
    line_data_->vertex_count += LineData::VertexForSingleLine;
    RendererStatistics::Get().vertex_count += LineData::VertexForSingleLine;
    RendererStatistics::Get().stats_2d_.lines++;
  }
  
  void Batch2DRenderer::DrawRect(const glm::vec3& p0, const glm::vec3& p2, const glm::vec4& color) {
    glm::vec3 p1 = glm::vec3(p2.x, p0.y, p0.z);
    glm::vec3 p3 = glm::vec3(p0.x, p2.y, p2.z);
    
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
  }
  
  void Batch2DRenderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
  }
  
  void Batch2DRenderer::DrawRect(const glm::mat4& transform, const glm::vec4& color) {
    glm::vec3 line_vertices[4];
    for (size_t i = 0; i < 4; i++)
      line_vertices[i] = transform * quad_data_->vertex_base_position[i];
    
    DrawLine(line_vertices[0], line_vertices[1], color);
    DrawLine(line_vertices[1], line_vertices[2], color);
    DrawLine(line_vertices[2], line_vertices[3], color);
    DrawLine(line_vertices[3], line_vertices[0], color);
  }
    
} // namespace ikan
