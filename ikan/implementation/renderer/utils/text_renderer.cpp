//
//  text_renderer.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "text_renderer.hpp"
#include "renderer/graphics/shader.hpp"
#include "renderer/graphics/pipeline.hpp"
#include "renderer/graphics/texture.hpp"
#include "renderer/utils/renderer.hpp"
#include "renderer/utils/renderer_stats.hpp"

namespace ikan {
  
  struct TextData {
    /// Vertex Information for rendering Text
    struct Vertex {
      glm::vec3 position;
      glm::vec4 color;
      glm::vec2 texture_coord;
      float texture_index;
      int32_t object_id;
    };
    
    // Fixed Constants
    static constexpr uint32_t VertexForSingleChar = 6;
    
    /// Renderer Data storage
    std::shared_ptr<Pipeline> pipeline;
    std::shared_ptr<VertexBuffer> vertex_buffer;
    std::shared_ptr<Shader> shader;
    
    std::array<std::shared_ptr<CharTexture>, MaxTextureSlotsInShader> char_textures;
    
    /// Map to store the Char Texture for each character
    std::map<char, std::shared_ptr<CharTexture>> char_texture_map;
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertex_buffer_base_ptr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertex_buffer_ptr = nullptr;
    
    /// Store the Base Texture coordinga
    glm::vec2 base_texture_coords[VertexForSingleChar];
    
    uint32_t num_slots_used = 0;
    
    virtual ~TextData() {
      delete [] vertex_buffer_base_ptr;
      vertex_buffer_base_ptr = nullptr;
    }
  };
  static std::unique_ptr<TextData> text_data_;

  void TextRenderer::Initialise() {
    text_data_ = std::make_unique<TextData>();
    
    // Allocating the memory for vertex Buffer Pointer
    text_data_->vertex_buffer_base_ptr = new TextData::Vertex[TextData::VertexForSingleChar * MaxTextureSlotsInShader];
    
    // Create Pipeline instance
    text_data_->pipeline = Pipeline::Create();
    
    // Create vertes Buffer
    text_data_->vertex_buffer = VertexBuffer::Create(sizeof(TextData::Vertex) * TextData::VertexForSingleChar * MaxTextureSlotsInShader);
    text_data_->vertex_buffer->AddLayout({
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Color",     ShaderDataType::Float4 },
      { "a_TexCoords", ShaderDataType::Float2 },
      { "a_TexIndex",  ShaderDataType::Float },
      { "a_ObjectID",  ShaderDataType::Int },
    });
    text_data_->pipeline->AddVertexBuffer(text_data_->vertex_buffer);
    
    // Settingup shader
    text_data_->shader = Renderer::GetShader(DirectoryManager::CoreAsset("shaders/text_shader.glsl"));
    
    // Base Texture coordinate for Char rendering
    text_data_->base_texture_coords[0] = { 0.0f, 0.0f };
    text_data_->base_texture_coords[1] = { 0.0f, 1.0f };
    text_data_->base_texture_coords[2] = { 1.0f, 1.0f };
    text_data_->base_texture_coords[3] = { 0.0f, 0.0f };
    text_data_->base_texture_coords[4] = { 1.0f, 1.0f };
    text_data_->base_texture_coords[5] = { 1.0f, 0.0f };
    
    IK_CORE_TRACE(LogModule::TextRenderer, "Initialised the Text Renderer");
    IK_CORE_TRACE(LogModule::TextRenderer, "  Vertex Buffer Used  {0} B",
                  TextData::VertexForSingleChar * sizeof(TextData::Vertex) * MaxTextureSlotsInShader);
    IK_CORE_TRACE(LogModule::TextRenderer, "  Shader used        | {0}", text_data_->shader->GetName());
  }
  
  void TextRenderer::Shutdown() {
    
  }
  
  void TextRenderer::LogData() {
    IK_CORE_INFO(LogModule::TextRenderer,"    Text Renderer Data ...");
    
    IK_CORE_INFO(LogModule::TextRenderer,"        Unique characters per Batch       | {0}", MaxTextureSlotsInShader);
    IK_CORE_INFO(LogModule::TextRenderer,"        Vertex Buffer Used                | {0} B",
                TextData::VertexForSingleChar * sizeof(TextData::Vertex) * MaxTextureSlotsInShader);
    IK_CORE_INFO(LogModule::TextRenderer,"        Shader used                       | {0}", text_data_->shader->GetName());
  }

} // namespace ikan
