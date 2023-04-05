//
//  layer_stack.cpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#include "layer_stack.hpp"

namespace ikan {
  
  LayerStack::LayerStack() {
    IK_CORE_TRACE(LogModule::LayerStack, "Creating Layerstack ...");
  }
  
  LayerStack::~LayerStack() {
    IK_CORE_TRACE(LogModule::LayerStack, "Destroying Layerstack !!!");
    IK_CORE_TRACE(LogModule::LayerStack, "Deleting all Layers from Layerstack");
    
    // Delete and deteach all the layers from stack
    for (auto& layer : layers_) {
      IK_CORE_TRACE(LogModule::LayerStack, "Destroying the {0} Layer from the stack", layer->GetName().c_str());
      
      layer->Detach();
    }
    
    layers_.erase(layers_.cbegin(), layers_.cend());
  }

  void LayerStack::PushLayer(const std::shared_ptr<Layer>& layer) {
    if (std::find(layers_.begin(), layers_.end(), layer) != layers_.end())
      return;
    
    IK_CORE_TRACE(LogModule::LayerStack, "Pushing the {0} Layer in the stack at position {1}. Total Layers added {2}",
                  layer->GetName().c_str(), layer_insert_index_, ++number_of_layers_);
    
    layers_.emplace(layers_.begin() + layer_insert_index_, layer);
    layer_insert_index_++;
    layer->Attach();
  }
  
  void LayerStack::PopLayer(const std::shared_ptr<Layer>& layer) {
    IK_CORE_TRACE(LogModule::LayerStack, "Poping the {0} Layer in the stack from the index {1}. Total Layers left {2}",
                  layer->GetName().c_str(), layer_insert_index_, --number_of_layers_);
    
    auto it = std::find(layers_.begin(), layers_.begin() + layer_insert_index_, layer);
    if (it != layers_.begin() + layer_insert_index_) {
      layer->Detach();
      layers_.erase(it);
      layer_insert_index_--;
    }
  }

  void LayerStack::PushOverlay(const std::shared_ptr<Layer>& layer) {
    if (std::find(layers_.begin(), layers_.end(), layer) != layers_.end())
      return;
    
    IK_CORE_TRACE(LogModule::LayerStack, "Pushing the {0} Layer in the stack at the end. Total Layers added {1}",
                  layer->GetName().c_str(), ++number_of_layers_);

    layers_.emplace_back(layer);
    layer->Attach();
  }
  
  void LayerStack::PopOverlay(const std::shared_ptr<Layer>& layer) {
    IK_CORE_TRACE(LogModule::LayerStack, "Poping the {0} Layer in the stack from the end. Total Layers left {1}",
                  layer->GetName().c_str(), --number_of_layers_);

    // Search the layer from the stack the detach/delete it
    if (auto it = std::find(layers_.begin(), layers_.end(), layer);
        it != layers_.end()) {
      layer->Detach();
      layers_.erase(it);
    }
  }
  
} // namespace ikan
