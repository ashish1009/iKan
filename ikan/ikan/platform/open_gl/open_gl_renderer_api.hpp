//
//  open_gl_renderer_api.hpp
//  ikan
//
//  Created by Ashish . on 31/03/23.
//

#pragma once

// This file includes the implenentation of Renderer API Class

#include "renderer/graphics/renderer_api.hpp"

namespace ikan {
  
  /// This class the implementation of Renderer API
  class OpenGLRendererAPI : public RendererAPI {
  public:
    /// This is the default constructor of Rendere API
    OpenGLRendererAPI();
    /// This is the default destructor ot Renderer API
    virtual ~OpenGLRendererAPI() noexcept;
    
    /// This function initialize the respective renderer api
    void Initialise() const override;
    /// This function shutdowns the respectiev renderer api
    void Shutdown() const override;

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererAPI);
  };
  
} // namesapce ikan
