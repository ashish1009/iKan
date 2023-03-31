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
    /// This function creates the instance of renderer API based on the supported API
    static std::unique_ptr<RendererAPI> Create();

    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
    
    /// This function initialize the respective renderer api
    virtual void Initialise() const = 0;
    /// This function shutdowns the respectiev renderer api
    virtual void Shutdown() const = 0;
  };
  
} // namespace ikan
