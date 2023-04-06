//
//  buffers.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

namespace ikan {
  
  /// This structure stores the data in a buffer as bytes. Allocate the memory in heap on cosntruction and de-allocate the memory on destruction
  struct Buffer {
  public:
    // Member variables
    std::byte* data = nullptr;
    uint32_t size = 0;
    
    constexpr Buffer() = default;
    /// This constructor assign the pointer comming in the argument as 'data'
    /// - Parameters:
    ///   - data: data pointer as std bytes:
    ///   - size: size of data:
    Buffer(std::byte* data, uint32_t size) : data(data), size(size) {}
    /// This is the copy constructor thet deep copies the data from 'other'
    /// - Parameter other: other Buffer:
    Buffer(const Buffer& other) : size(other.size) {
      data = new std::byte[size];
      memcpy(data, other.data, size);
      allocated_bytes_ += size;
    }
    /// This is the move constructor that moves the buffer from other to this
    /// - Parameter other: other Buffer:
    Buffer(Buffer&& other) : data(other.data),  size(other.size) {
      other.size = 0;
      other.data = nullptr;
    }
    
    /// This destructor destroy the memory of buffer and clear it
    ~Buffer() { Clear(); }
    
    /// This functions deletes the Buffer and make size 0
    void Clear() {
      if (data)
        Dealocate();
      
      size = 0;
      data = nullptr;
    }
    
    /// This function sets the memory of buffer to 0
    void ZeroInitialize() {
      if (data) memset(data, 0, size);
    }
    /// This funciton writes the data in the buffer
    /// - Parameters:
    ///   - data: Data point to be written in the buffer as void*:
    ///   - size: Size of data to be copied:
    ///   - offset:  Offset from where data to be copied. Offset from 'data':
    void Write(void* new_data, uint32_t allocated_size, uint32_t offset = 0) {
      if (allocated_size == 0) return;
      IK_ASSERT(offset + allocated_size <= size or offset > size, "Buffer overflow!");
      memcpy(data + offset, new_data, allocated_size);
    }
    /// This function allocates the memory to buffer of size 'size'
    /// - Parameter - Size of new buffer:
    void Allocate(uint32_t allocated_size) {
      if (allocated_size == 0) return;
      
      Clear();
      size = allocated_size;
      data = new std::byte[size];
      
      allocated_bytes_ += size;
    }
    
    /// This function returns the size of buffer
    constexpr uint32_t GetSize() const { return size; }
    
    /// This function returns true if Data is not nullptr
    operator bool() const { return data; }
    /// This function returns the byte reference at position index
    /// - Parameter index: Index of position:
    std::byte& operator[](uint32_t index) { return data[index]; }
    /// This function returns the byte value at position index
    /// - Parameter index: Index of position:
    std::byte operator[](uint32_t index) const { return data[index]; }
    
    /// This function assign the 'other' buffer to this
    /// - Parameter other: Assign Buffer other to this Buffer:
    /// - Important: New memory allocation is done (Deep Copy)
    Buffer& operator =(const Buffer& other) {
      Clear();
      Allocate(other.size);
      Write((void*)other.data, other.size);
      return *this;
    }
    /// This function moves the 'other' buffer to this
    /// - Parameter other: Assign Buffer other to this Buffer:
    /// - Important: New memory allocation is done (Deep Copy)
    Buffer& operator =(Buffer&& other) {
      Clear();
      
      size = other.size;
      data = other.data;
      
      other.size = 0;
      other.data = nullptr;
      
      return *this;
    }
    
    /// This function creates new instance of Buffer. Copy the 'data' and return the newly created buffer
    /// - Parameters:
    ///   - data: Data pointer (void*) to be copied:
    ///   - size: size of data:
    [[nodiscard]] static Buffer Copy(void* data, uint32_t size) {
      Buffer buffer;
      
      if (size == 0)
        return buffer;
      
      buffer.Allocate(size);
      memcpy(buffer.data, data, size);
      return buffer;
    }
    /// This function returns the total allocated size in entire program
    static uint32_t GetTotalAllocations() { return allocated_bytes_; }
    
    /// This function reads the buffer at specific offset as T
    /// - Parameter - offset from where buffer to be read:
    template<typename T> T& Read(uint32_t offset) { return *(T*)(data + offset); }
    /// This function reads thr buffer from begining as T
    template<typename T> constexpr T* As() { return (T*)data; }
    
  private:
    /// This function dealocate the memory of buffer
    void Dealocate() {
      allocated_bytes_ -= size;
      delete[] data;
    }
    
    // Stores the Total allocation made in program using Buffer class
    inline static uint32_t allocated_bytes_ = 0;
  };
  
} // namespace ikan
