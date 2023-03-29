//
//  ref.hpp
//  ikan
//
//  Created by Ashish . on 28/03/23.
//

#pragma once

#include <stdint.h>
#include <atomic>
#include "Memory.h"

namespace ikan {
  
  namespace ref_utils {
    void AddToLiveReferences(void* instance);
    void RemoveFromLiveReferences(void* instance);
    bool IsLive(void* instance);
  }
  
  /// This class is the base class for any class T which will be created as Reference count (Ref<T>)
  class RefCounted {
  public:
    /// This function increase the reference count of class
    void IncRefCount() const { ++ref_count_; }
    /// This function decrease the reference count of class
    void DecRefCount() const { --ref_count_; }
    /// This function return the reference count of class
    uint32_t GetRefCount() const { return ref_count_.load(); }
  private:
    mutable std::atomic<uint32_t> ref_count_ = 0;
  };

  template<typename T>
  class Ref {
  public:
    /// This constructur creates the null Reference pointer of class T
    Ref() : instance_(nullptr) { }
    /// This constructur creates the null Reference pointer of class T
    Ref(std::nullptr_t n) : instance_(nullptr) { }
    
    /// This constructor creates the Renference pointer of class T (copy the pointer address)
    /// - Parameter instance: instance pointer
    Ref(T* instance) : instance_(instance) {
      static_assert(std::is_base_of<RefCounted, T>::value, "Class is not derived from RefCounted!");
      IncRef();
    }

    template<typename T2>
    /// This constructor copies the reference pointer oc class T2 to as reference pointer of class T
    /// - Note: we do not deep copy the date, just type cast the pointers
    /// - Parameter other: reference pointer of class T2
    Ref(const Ref<T2>& other) {
      instance_ = (T*)other.instance_;
      IncRef();
    }
    
    template<typename T2>
    /// This constructor moves the reference pointer of class T2 to as reference pointer of class T
    /// - Note: we do not deep copy the date, just type cast the pointers and make 'other' instance as null
    /// - Parameter other: reference pointer of class T2
    Ref(Ref<T2>&& other) {
      instance_ = (T*)other.instance_;
      other.instance_ = nullptr;
    }
    
    /// This constructor copies the reference pointer of class T and increase the counter
    /// - Note: we do not deep copy the date, just type cast the pointers
    /// - Parameter other: other reference pointer of type T
    Ref(const Ref<T>& other) : instance_(other.instance_) {
      IncRef();
    }

    /// This Destructor Decrement the Reference counting. If counting is 0 then destory the pointer
    ~Ref() { DecRef(); }
    
    /// This operator assign the nullptr to the instance
    /// - Parameter nullptr_t: nullptr
    Ref& operator=(std::nullptr_t) {
      DecRef();
      instance_ = nullptr;
      return *this;
    }
    
    /// This operator = copies the reference pointer of class T and increase the counter
    /// - Note: we do not deep copy the date, just type cast the pointers
    /// - Parameter other: other reference pointer of type T
    Ref& operator=(const Ref<T>& other) {
      other.IncRef();
      DecRef();
      
      instance_ = other.instance_;
      return *this;
    }

    template<typename T2>
    /// This operator copies the reference pointer oc class T2 to as reference pointer of class T
    /// - Note: we do not deep copy the date, just type cast the pointers
    /// - Parameter other: reference pointer of class T2
    Ref& operator=(const Ref<T2>& other) {
      other.IncRef();
      DecRef();
      
      instance_ = other.instance_;
      return *this;
    }
    
    template<typename T2>
    /// This operator moves the reference pointer of class T2 to as reference pointer of class T
    /// - Note: we do not deep copy the date, just type cast the pointers and make 'other' instance as null
    /// - Parameter other: reference pointer of class T2
    Ref& operator=(Ref<T2>&& other) {
      DecRef();
      
      instance_ = other.instance_;
      other.instance_ = nullptr;
      return *this;
    }

    operator bool() { return instance_ != nullptr; }
    operator bool() const { return instance_ != nullptr; }
    
    /// This operator return the Ref<T> as raw pointer
    T* operator->() { return instance_; }
    /// This operator return the Ref<T> as const raw pointer
    const T* operator->() const { return instance_; }
    
    /// This operator return the Ref<T> as refernce value
    T& operator*() { return *instance_; }
    /// This operator return the Ref<T> as const refernce value
    const T& operator*() const { return *instance_; }
    
    /// This operator compare the pointers of this to other
    /// - Parameter other: other Ref<T>
    bool operator==(const Ref<T>& other) const { return instance_ == other.instance_; }
    /// This operator compare the pointers of this to other
    /// - Parameter other: other Ref<T>
    bool operator!=(const Ref<T>& other) const { return !(*this == other); }

    /// This fucntion return the Ref<T> as raw pointer
    T* Raw() { return  instance_; }
    /// This function return the Ref<T> as const raw pointer
    const T* Raw() const { return  instance_; }
    
    /// This function decrement the reference pointer counter by 1
    /// - Parameter instance: instance to be assigned to Ref<T> instance
    void Reset(T* instance = nullptr) {
      DecRef();
      instance_ = instance;
    }
    
    /// This function compares the data of instances
    /// - Parameter other: other Ref<T> to be compared
    bool EqualsObject(const Ref<T>& other) {
      if (!instance_ || !other.instance_)
        return false;
      
      return *instance_ == *other.instance_;
    }

    template<typename T2>
    /// This function returns the Ref<T> as Ref<T2>
    /// - Note: Just type cast the pointer
    Ref<T2> As() const { return Ref<T2>(*this); }

    template<typename... Args>
    /// This API creates a new Ref<T> instance as default cunstructor of T
    /// - Parameter args: arguments of T Constructors...
    static Ref<T> Create(Args&&... args) {
      return Ref<T>(new T(std::forward<Args>(args)...));
    }

    /// This API Creates the new Ref type T and copy the reference pointer of type T from 'other' Ref to current pointer.
    /// - Note: But do not increase the reference counting
    /// - Parameter other: other referene pointer
    static Ref<T> CopyWithoutIncrement(const Ref<T>& other) {
      Ref<T> result = nullptr;
      result->instance_ = other.instance_;
      return result;
    }
    
  private:
    /// This function increment the reference counter of Ref<T> class
    void IncRef() const {
      if (instance_) {
        instance_->IncRefCount();
        ref_utils::AddToLiveReferences((void*)instance_);
      }
    }

    /// This function decrement the reference counter of Ref<T> class. If count is 0 then destroy the pointer
    void DecRef() const {
      if (instance_) {
        instance_->DecRefCount();
        if (instance_->GetRefCount() == 0) {
          delete instance_;
          ref_utils::RemoveFromLiveReferences((void*)instance_);
          instance_ = nullptr;
        }
      }
    }

    template<class T2>
    friend class Ref;
    mutable T* instance_;
  };
  
} // namespace ikan
