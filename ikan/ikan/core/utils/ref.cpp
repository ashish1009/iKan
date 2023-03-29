//
//  ref.cpp
//  ikan
//
//  Created by Ashish . on 28/03/23.
//

#include "ref.hpp"

namespace ikan {
  
  static std::unordered_set<void*> live_references_;
  static std::mutex live_reference_mutex;
  
  namespace ref_utils {
    
    void AddToLiveReferences(void* instance) {
      std::scoped_lock<std::mutex> lock(live_reference_mutex);
      IK_CORE_ASSERT(instance);
      live_references_.insert(instance);
    }
    
    void RemoveFromLiveReferences(void* instance) {
      std::scoped_lock<std::mutex> lock(live_reference_mutex);
      IK_CORE_ASSERT(instance);
      IK_CORE_ASSERT(live_references_.find(instance) != live_references_.end());
      live_references_.erase(instance); }
    
    bool IsLive(void* instance) {
      IK_CORE_ASSERT(instance);
      return live_references_.find(instance) != live_references_.end();
    }
    
  } // namespace ref_utils
  
} // namespace ikan
