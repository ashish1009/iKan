//
//  time_step.h
//  ikan
//
//  Created by Ashish . on 01/04/23.
//

#pragma once

namespace ikan {
  
  /// This is a class to store the time difference between two frames.
  /// - Note: This class only store the time stamp, it is not any counter to update the time on its own Time need to be updated each frame by either
  /// window or application
  class Timestep {
  public:
    /// This constructor creates the instance and store the time
    /// - Parameter time: Initial time to be set:
    Timestep(float time = 0.0f) : time_(time) {}
    /// Defatult construtor
    ~Timestep() = default;
    
    /// This function returns the Time as float
    operator float() const { return time_; }
    
    /// This function returns the time step (Time took to render 1 Frame) in Seconds
    float Seconds() const { return time_; }
    /// This function returns the time step (Time took to render 1 Frame) in mili Seconds
    float MilliSeconds() const { return time_ * 1000.0f; }
    /// This function returns the time step (Time took to render 1 Frame) in micro Seconds
    float MicroSeconds() const { return time_ * 1000000.0f; }
    
  private:
    float time_ = 0.0f;
  };
  
} // namespace ikan
