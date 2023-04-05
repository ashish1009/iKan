//
//  ikan_common.h
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

namespace ikan {
  
  /// This Emum defines the supported Operating system for ikan Engine
  enum class OperatingSystem : uint8_t {
    None = 0, Mac = 1
    // Add More supported OS
  };
  
}

// ----------------------------------
// Common Macros
// ----------------------------------
/// This MACRO Shifts the LSB bit of number 1 at position x
#define BIT(x)          (1 << x)

// --------------------------------------------------------
// Macro Wrapper to define Enums and its String to print
// --------------------------------------------------------
#define VAL(name) name ,
#define STRING(name) #name,

#define CreateEnum(Name) \
enum class Name { Name(VAL) }; \
static const char* Name##String[] = { Name(STRING) }; \

// -------------------------------------------
// Copy and Move Constructors and Operators
// -------------------------------------------
/// This MACRO Deletes the Constructors (Default, Copy and Move ) and Operator = (Copy and Move) for any class to make pure static class
#define MAKE_PURE_STATIC(x) \
x() = delete; \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator =(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO deletes the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x to make a class singleton.
/// NOTE: default constructor still need to be private
#define DELETE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO Defines the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x
#define DEFINE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&); \
x(x&&); \
x& operator=(const x&); \
x& operator =(x&&); \
