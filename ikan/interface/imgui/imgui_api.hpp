//
//  Imgui_api.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
  
  /// Imgui Wrapper class
  class ImguiAPI {
  public:
    /// This function Enables the docking in Imgui. To be called each frame before every imgui rendering;
    static void StartDcocking();
    /// This function Disable the docking in Imgui. To be called each frame after every imgui rendering;
    static void EndDcocking();

    MAKE_PURE_STATIC(ImguiAPI);
  };
  
}
