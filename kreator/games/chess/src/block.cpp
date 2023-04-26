//
//  block.cpp
//  kreator
//
//  Created by Ashish . on 26/04/23.
//

#include "block.hpp"
#include "common.hpp"

namespace chess {
  
  Block::Block() {
    static int num_blocks = 0;
    CHESS_LOG("Creating Block Instance {0}", num_blocks++);
  }

  Block::~Block() {
    CHESS_LOG("Destroying Block Instance");
  }

} // namespace chess
