//
//  core_logger_api.h
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/debug/logger.hpp"

#ifdef IK_ENABLE_LOG

#define IK_CORE_TRACE(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Trace, tag, __VA_ARGS__)
#define IK_CORE_DEBUG(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Debug, tag, __VA_ARGS__)
#define IK_CORE_INFO(tag, ...)     ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Info, tag, __VA_ARGS__)
#define IK_CORE_WARN(tag, ...)     ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Warning, tag, __VA_ARGS__)
#define IK_CORE_ERROR(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Error, tag, __VA_ARGS__)
#define IK_CORE_CRITICAL(tag, ...) ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Core, ::ikan::Logger::Level::Critical, tag, __VA_ARGS__)

#else

#define IK_CORE_TRACE(...)
#define IK_CORE_DEBUG(...)
#define IK_CORE_INFO(...)
#define IK_CORE_WARN(...)
#define IK_CORE_ERROR(...)
#define IK_CORE_CRITICAL(...)

#endif
