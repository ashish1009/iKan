//
//  mario.cpp
//  kreator
//
//  Created by Ashish . on 13/04/23.
//

#include "mario.hpp"

namespace mario {
  
  void Mario::Init(const std::shared_ptr<Scene> scene) {
    scene_ = scene;
    
    timer_ = 0;
    time_left_ = MaxTime;
  }
  
  void Mario::Update(Timestep ts) {
    if (is_playing_) {
      // Timer
      timer_ += ts;
      time_left_ = MaxTime - (uint32_t)timer_;
    }

    // Score and All text
    TextRenderer::BeginBatch(FixedCamera::projection);
    
    text_data_.Render("MARIO", 0, 0);
    text_data_.Render(std::to_string(0), 1, 0);
        
    text_data_.Render("x " + std::to_string(0), 1, 1);
    
    text_data_.Render("WORLD", 0, 2);
    text_data_.Render(std::to_string(world_) + " - " + std::to_string(level_), 1, 2);
    
    text_data_.Render("TIME", 0, 3);
    text_data_.Render(std::to_string(time_left_), 1, 3);
    
    TextRenderer::EndBatch();
  }
  
  void Mario::SetViewportSize(uint32_t width, uint32_t height) {
    text_data_.SetViewport(width, height);
  }

} // namespace mario
