//
//  layer_stack.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/layer.hpp"

namespace ikan {
  
  /// Stack to store all the Layers used by application Responsible for Insertion, deletion and Iteration of layers Storing data as std::vector
  class LayerStack {
  public:
    /// Default constructor of layer stack. Just creating isntance
    LayerStack();
    /// Destructor of layer stack. Remove all the layer from stack and detach them from application
    ~LayerStack();
    
    /// This function pushes the layer in the Stack at last inserted layer index
    /// - Parameter layer: layer shaerd pointer to be inserted
    void PushLayer(const std::shared_ptr<Layer>& layer);
    /// This function erases the layer from the Stack from last inserted layer index
    /// - Parameter layer layer shared pointer to be deleted
    void PopLayer(const std::shared_ptr<Layer>& layer);

    /// This function pushes the layer in the Stack at the end
    /// - Parameter layer: layer shaerd pointer to be inserted
    void PushOverlay(const std::shared_ptr<Layer>& layer);
    /// This function erases the layer from the Stack from the end
    /// - Parameter layer layer shared pointer to be deleted
    void PopOverlay(const std::shared_ptr<Layer>& layer);

    std::vector<std::shared_ptr<Layer>>::iterator begin() { return layers_.begin(); }
    std::vector<std::shared_ptr<Layer>>::iterator end() { return layers_.end(); }
    std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return layers_.rbegin(); }
    std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return layers_.rend(); }
    
    std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return layers_.begin(); }
    std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return layers_.end(); }
    std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
    std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(LayerStack);
    
  private:
    uint32_t layer_insert_index_ = 0;
    uint32_t number_of_layers_ = 0;
    std::vector<std::shared_ptr<Layer>> layers_;
  };
  
} // namespace ikan
