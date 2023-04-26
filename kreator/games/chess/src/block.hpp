//
//  block.hpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#pragma once

namespace chess {
  
#define BlockRef std::shared_ptr<Block>
  
  class Block {
  public:
    /// This Constructor creates the Block Data
    Block();
    /// This Constructor destroy the Block Data
    ~Block();
  private:
  };
    
} // namespace chess
