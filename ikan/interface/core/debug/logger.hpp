//
//  logger.hpp
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

#include <spdlog/spdlog.h>

namespace ikan {
  
/// This enum stores the moule names of ikan Logs (Holds only ikan engine log module names)
#define LogModule(f) \
  f(None) \
  f(Event) f(StringUtils) \
  f(Application) f(Window) f(DirectoryManager) f(LayerStack) \
  f(Renderer) f(GraphicsContext) f(Batch2DRenderer) f(TextRenderer) \
  f(Pipeline) f(VertexBuffer) f(IndexBuffer) f(Shader) f(Texture) f(FrameBuffer) f(CharTexture) \
  f(EditorCamera) f(SceneCamera) \
  f(ContentBrowserPanel) \
f(Scene) f(ScenePanelManager) f(SceneSerializer) f(Entity) \
  f(Imgui) \
  
CreateEnum(LogModule);
  
  /// This class is the wrapper the wrapepr class for logging.
  /// - Note: This wrapper class is using submodule spd-log from "https://github.com/gabime/spdlog"
  class Logger {
  public:
    /// This enum hold the Log level of ikan engine
    enum class Level : uint8_t {
      Trace, Debug, Info, Warning, Error, Critical
    };
    /// This enum hold the Project module of log
    enum class Type : uint8_t {
      Core, Client
    };

    /// This stores the Log Module details. Logs can be filtered and passed based on the modules
    struct TagDetails {
      bool enabled = true; // Flag to enable the Module log
      Level level_filter = Level::Trace; // Filter of log level for this module
    };

    /// This function initializes the spd logger. Create instance for both core and client. Sets the core and client log levels as 'core_level'
    /// and 'client_level'. Create a file at path 'log_file_path' to save the logs in a file. Also set the format for the logger
    /// - Current format is [%-8l | %-4n | %v
    ///   - %l : Log lebel string (-8 measn width reserved for the same)
    ///   - %n : Logger Type (core or client) (-4 is width reserved for the same)
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
    
    /// Get the detail of a module tag
    /// - Parameter - tag of log module:
    static TagDetails& GetDetail(const std::string& tag);

    /// This functoin Disables the module log
    /// - Parameter tag: tag name to be disabled:
    static void DisableModule(LogModule tag);
    /// This functoin Enables module log
    /// - Parameter tag: tag name to be enabled:
    static void EnableModule(LogModule tag);

    /// This function returns the shared pointer of Core log instance
    static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return core_logger_; }
    /// This function returns the shared pointer of Client log instance
    static std::shared_ptr<spdlog::logger>& GetClientLogger() { return client_logger_; }
    
    /// This function returns the shared pointer of Core log instance
    static Level GetCoreLevel() { return core_level_; }
    /// This function returns the shared pointer of Client log instance
    static Level GetClientLevel() { return client_level_; }
    
    template<typename... Args>
    /// This function stores the log with tag of module
    /// - Parameters:
    ///   - type: of log project
    ///   - level: of log
    ///   - tag: of module
    ///   - Log: string with argument
    static void PrintMessage(Type type, Level level, LogModule tag, Args&&... args) {
      PrintMessageImpl(type, level, GetModuleName(tag), std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    /// This function stores the log with tag of module
    /// - Parameters:
    ///   - type: of log project
    ///   - level: of log
    ///   - tag: of module
    ///   - Log: string with argument
    static void PrintMessage(Type type, Level level, std::string_view tag, Args&&... args) {
      PrintMessageImpl(type, level, GetModuleName(tag), std::forward<Args>(args)...);
    }

  private:
    MAKE_PURE_STATIC(Logger);
    static std::shared_ptr<spdlog::logger> core_logger_, client_logger_;
    inline static std::map<std::string, TagDetails> enabled_tags_;
    inline static Level core_level_, client_level_;

    /// this functun return the tag stored in logger
    /// - Parameter - tag of log module:
    static bool HasTag(const std::string& tag) { return enabled_tags_.find(tag) != enabled_tags_.end(); }

    /// This function returns the module name string from string view
    /// - Parameter module_tag: module name as string view
    static std::string GetModuleName(const std::string_view module_tag) { return std::string(module_tag); }
    /// This function returns the module name string from 'LogModule'
    /// - Parameter module_tag: module name as 'LogModule'
    static std::string GetModuleName(LogModule module_tag) { return LogModuleString[(uint32_t)module_tag]; }
    
    template<typename... Args>
    /// This function stores the log with tag of module
    /// - Parameters:
    ///   - type: of log project
    ///   - level: of log
    ///   - tag: of module
    ///   - Log: string with argument
    static void PrintMessageImpl(Type type, Level level, std::string tag, Args&&... args) {
      static const uint32_t MaxTagLegth = 20;
      
      const auto& detail = GetDetail(std::string(tag));
      if (detail.enabled && detail.level_filter <= level) {
        auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
        std::string logString = std::string("{0}" + std::string(size_t(MaxTagLegth - tag.size()), ' ') + std::string("] | {1}"));
        switch (level) {
          case Level::Debug:
            logger->debug(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Trace:
            logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Info:
            logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Warning:
            logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Error:
            logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Critical:
            logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
            break;
        }
      }
    }
  };
  
} // namespace ikan

#ifdef IK_ENABLE_LOG

#define IK_TRACE(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Trace, tag, __VA_ARGS__)
#define IK_DEBUG(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Debug, tag, __VA_ARGS__)
#define IK_INFO(tag, ...)     ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Info, tag, __VA_ARGS__)
#define IK_WARN(tag, ...)     ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Warning, tag, __VA_ARGS__)
#define IK_ERROR(tag, ...)    ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Error, tag, __VA_ARGS__)
#define IK_CRITICAL(tag, ...) ::ikan::Logger::PrintMessage(::ikan::Logger::Type::Client, ::ikan::Logger::Level::Critical, tag, __VA_ARGS__)

#else

#define IK_TRACE(...)
#define IK_DEBUG(...)
#define IK_INFO(...)
#define IK_WARN(...)
#define IK_ERROR(...)
#define IK_CRITICAL(...)

#endif
