//
//  scriptable_entity.cpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#include "scriptable_entity.hpp"
#include "scene/components.hpp"

namespace ikan {
  
  void ScriptManager::UpdateScript(NativeScriptComponent* sc, const std::string& script_name, ScriptLoaderFn loader_function) {
    if (loader_function) {
      bool script_loaded = loader_function(sc, script_name);
      IK_ASSERT(script_loaded, "Invalid Script name");
    }
    else {
      sc->Bind<ikan::ScriptableEntity>();
    }
  }
  
} // namespace ikan
