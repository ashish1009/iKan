//
//  open_gl_renderer_buffer.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_renderer_buffer.hpp"
#include "renderer/utils/renderer_stats.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"
#include <glad/glad.h>

namespace ikan {
  
  // --------------------------------------------------------------------------
  // Vertex Buffer
  // --------------------------------------------------------------------------
  OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) : size_(size) {
    IDManager::GetBufferId(renderer_id_);
    RendererStatistics::Get().vertex_buffer_size += size_;
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW);    
    
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer with Data ...");
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", renderer_id_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", size_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().vertex_buffer_size);
  }
  
  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) : size_(size) {
    IDManager::GetBufferId(renderer_id_);
    RendererStatistics::Get().vertex_buffer_size += size_;
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size_, nullptr, GL_DYNAMIC_DRAW);
    
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer without Data ...");
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", renderer_id_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", size_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().vertex_buffer_size);
  }
  
  OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Destroying Open GL Vertex Buffer ...");
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", renderer_id_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", size_);
    IK_CORE_DEBUG(LogModule::VertexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().vertex_buffer_size);

    RendererStatistics::Get().vertex_buffer_size -= size_;
    IDManager::RemoveBufferId(renderer_id_);
  }
  
  void OpenGLVertexBuffer::SetData(void* data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  }
  
  void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  }
  
  void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size)
  : size_(size), count_(size_ / sizeof(uint32_t)) {
    IDManager::GetBufferId(renderer_id_);
    RendererStatistics::Get().index_buffer_size += size_;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, data, GL_STATIC_DRAW);
        
    IK_CORE_DEBUG(LogModule::IndexBuffer, "Creating Open GL Index Buffer ...");
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Renderer ID       {0}", renderer_id_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Number of Indices {0}", count_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Size              {0} B", size_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().index_buffer_size);
  }
  
  OpenGLIndexBuffer::~OpenGLIndexBuffer() noexcept {
    RendererStatistics::Get().index_buffer_size -= size_;
    IDManager::RemoveBufferId(renderer_id_);
    
    IK_CORE_DEBUG(LogModule::IndexBuffer, "Destroying Open GL Index Buffer ...");
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Renderer ID       {0}", renderer_id_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Number of Indices {0}", count_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Size              {0} B", size_);
    IK_CORE_DEBUG(LogModule::IndexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().index_buffer_size);
  }
  
  void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  }
  
  void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  
} // namespace ikan
