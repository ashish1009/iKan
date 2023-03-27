//
//  logger.hpp
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

#include <spdlog/spdlog.h>

namespace ikan {
  
  /// This class is the wrapper the wrapepr class for logging.
  /// - Note: This wrapper class is using submodule spd-log from "https://github.com/gabime/spdlog"
  class Logger {
  public:
    /// This enum hold the Log level of ikan engine
    enum class Level : uint8_t {
      Trace, Debug, Info, Warning, Error, Critical
    };

    /// This function initializes the spd logger. Create instance for both core and client. Sets the core and client log levels as 'core_level'
    /// and 'client_level'. Create a file at path 'log_file_path' to save the logs in a file. Also set the format for the logger
    /// - Current format is [%T:%e:%f] [%-8l] [%-4n] : %v
    ///   where :
    ///   - %T : Time stamp as hh:mm:ss
    ///   - %e : Time stamp in milliseconds
    ///   - %f : Time stamp in microseconds
    ///   - %l : Log lebel string (-8 measn width reserved for the same)
    ///   - %n : Logger Type (core or client) (-4 is width reserved for the same)
    ///
    /// - Parameters:
    ///   - core_level: Core log level
    ///   - client_level: Client log level
    ///   - log_file_path: Saved log file Path (relative to executable binary, with file name and extenstion), where our log file to be saved
    /// - Note: Folders may or may not already existed. If any folder is missing from the path then new folder will be created in same hierarchy
    static void Init(Level core_level, Level client_level, const std::string& log_file_path);
    
    /// This function converts spd log level type to string for debug printing
    /// - Parameter - spd log level enum:
    static const char* GetLogLevelStringFromSpdLevel(spdlog::level::level_enum level);
    /// This function converts ikan log level to the spd log level
    /// - Parameter - ikan log level enum:
    static spdlog::level::level_enum GetSpdLevelFromIKanLevel(Logger::Level level);
    
    /// This function returns the shared pointer of Core log instance
    static std::shared_ptr<spdlog::logger>& GetCoreLogger();
    /// This function returns the shared pointer of Client log instance
    static std::shared_ptr<spdlog::logger>& GetClientLogger();

  private:
    MAKE_PURE_STATIC(Logger);
    static std::shared_ptr<spdlog::logger> core_logger_, client_logger_;
  };
  
}
