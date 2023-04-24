//
//  block_controller.hpp
//  kreator
//
//  Created by Ashish . on 24/04/23.
//

#pragma once

namespace mario {
  
  using namespace ikan;
  
  enum class BlockType {
    Empty, Coin, PowerUp, Star
  };
  
  class BlockScriptManager {
  public:
    /// This function Initialse the Block Scripts
    static void Init();
    /// This function Destorys the Block Scripts
    static void Shutdown();
    
    /// This function returns the script loader function for a specific Block
    /// - Parameter tag: type name of Block
    static ScriptLoaderFn GetLoaderFn(const std::string& tag);
    /// This function returns the type of Block from name
    /// - Parameter tag: type name of Block
    static BlockType GetType(const std::string& tag);
    /// This function returns the count of Item present in block
    /// - Parameter tag: type name of Block
    static uint32_t GetCount(const std::string& tag);
  };
  using BSM = BlockScriptManager;
  
} // namespace mario
