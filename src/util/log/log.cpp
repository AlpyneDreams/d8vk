#include "log.h"

#include "../util_env.h"

namespace dxvk {
  
  Logger::Logger(const std::string& file_name)
  : m_minLevel(getMinLogLevel()) {
    s_initialized = true;
    if (m_minLevel != LogLevel::None) {
      setLogFile(file_name);
    }
  }
  
  
  Logger::~Logger() { }
  
  
  void Logger::setLogFile(const std::string& file_name) {
    if (s_instance.m_minLevel != LogLevel::None) {
      auto path = getFileName(file_name);
      
      static std::string s_overrideFileName = "";
      if (!s_initialized) {
        // logger hasn't initialized yet.
        s_overrideFileName = path;
        return;
      } else if (!s_overrideFileName.empty()) {
        // logger initializing, and we have an override.
        path = s_overrideFileName;
        s_overrideFileName = "";
      }

      if (path == s_instance.m_fileName)
        return;

      bool empty = s_instance.m_fileStream && s_instance.m_fileStream.tellp() <= 0;

      if (!path.empty()) {
        // pause logging while we switch files
        std::lock_guard<dxvk::mutex> lock(s_instance.m_mutex);
        s_instance.m_fileStream = std::ofstream(str::topath(path.c_str()).c_str());
      } else {
        s_instance.m_fileStream.close();
      }

      if (empty && !s_instance.m_fileName.empty()) // don't leave empty log files
        std::remove(s_instance.m_fileName.c_str());

      s_instance.m_fileName = path;
    }
  }
 

  void Logger::trace(const std::string& message) {
    s_instance.emitMsg(LogLevel::Trace, message);
  }
  
  
  void Logger::debug(const std::string& message) {
    s_instance.emitMsg(LogLevel::Debug, message);
  }
  
  
  void Logger::info(const std::string& message) {
    s_instance.emitMsg(LogLevel::Info, message);
  }
  
  
  void Logger::warn(const std::string& message) {
    s_instance.emitMsg(LogLevel::Warn, message);
  }
  
  
  void Logger::err(const std::string& message) {
    s_instance.emitMsg(LogLevel::Error, message);
  }
  
  
  void Logger::log(LogLevel level, const std::string& message) {
    s_instance.emitMsg(level, message);
  }
  
  
  void Logger::emitMsg(LogLevel level, const std::string& message) {
    if (level >= m_minLevel) {
      std::lock_guard<dxvk::mutex> lock(m_mutex);
      
      static std::array<const char*, 5> s_prefixes
        = {{ "trace: ", "debug: ", "info:  ", "warn:  ", "err:   " }};
      
      const char* prefix = s_prefixes.at(static_cast<uint32_t>(level));

      std::stringstream stream(message);
      std::string       line;

      while (std::getline(stream, line, '\n')) {
        std::cerr << prefix << line << std::endl;

        if (m_fileStream)
          m_fileStream << prefix << line << std::endl;
      }
    }
  }
  
  
  LogLevel Logger::getMinLogLevel() {
    const std::array<std::pair<const char*, LogLevel>, 6> logLevels = {{
      { "trace", LogLevel::Trace },
      { "debug", LogLevel::Debug },
      { "info",  LogLevel::Info  },
      { "warn",  LogLevel::Warn  },
      { "error", LogLevel::Error },
      { "none",  LogLevel::None  },
    }};
    
    const std::string logLevelStr = env::getEnvVar("DXVK_LOG_LEVEL");
    
    for (const auto& pair : logLevels) {
      if (logLevelStr == pair.first)
        return pair.second;
    }
    
    return LogLevel::Info;
  }
  
  
  std::string Logger::getFileName(const std::string& base) {
    std::string path = env::getEnvVar("DXVK_LOG_PATH");
    
    if (path == "none")
      return "";

    if (!path.empty() && *path.rbegin() != '/')
      path += '/';

    std::string exeName = env::getExeBaseName();
    path += exeName + "_" + base;
    return path;
  }
  
}
