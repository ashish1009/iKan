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
    
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer ID {0} with Data of size {1} B ...", renderer_id_, size_);
  }
  
  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) : size_(size) {
    IDManager::GetBufferId(renderer_id_);
    RendererStatistics::Get().vertex_buffer_size += size_;
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size_, nullptr, GL_DYNAMIC_DRAW);
    
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer ID {0} without Data of size {1} B ...", renderer_id_, size_);
  }
  
  OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    IK_CORE_DEBUG(LogModule::VertexBuffer, "Destroying Open GL Vertex Buffer ID {0}, size {1} B !!!", renderer_id_, size_);
    
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
        
    IK_CORE_DEBUG(LogModule::IndexBuffer, "Creating Open GL Index Buffer ID {0} with count {1} and size {2} B ...",
                  renderer_id_, count_, size_);
  }
  
  OpenGLIndexBuffer::~OpenGLIndexBuffer() noexcept {
    RendererStatistics::Get().index_buffer_size -= size_;
    IDManager::RemoveBufferId(renderer_id_);
    
    IK_CORE_DEBUG(LogModule::IndexBuffer, "Destroying Open GL Index Buffer ID {0} with count {1} and size {2} B ...",
                  renderer_id_, count_, size_);
  }
  
  void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  }
  
  void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  
} // namespace ikan
