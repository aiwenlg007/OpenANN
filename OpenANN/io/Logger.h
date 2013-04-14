#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#ifndef NDEBUG

#define OPENANN_OUTPUT(msg) std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl;
#define OPENANN_TRACE(msg) std::cerr << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl;

#else // NDEBUG

#define OPENANN_OUTPUT(msg) std::cout << msg << std::endl;
#define OPENANN_TRACE(msg)

#endif // NDEBUG


#ifndef OPENNANN_LOGLEVEL 
  #define OPENANN_LOGLEVEL OpenANN::Log::kDebug 
#endif // OPENANN_LOGLEVEL

#define OPENANN_LOG(level) \
    if(level > OPENANN_LOGLEVEL) ; \
    else if(level > OpenANN::Log::Level()) ; \
    else OpenANN::Log().get(level)

#define OPENANN_DEBUG OPENANN_LOG(OpenANN::Log::kDebug)
#define OPENANN_INFO OPENANN_LOG(OpenANN::Log::kInfo)
#define OPENANN_ERROR OPENANN_LOG(OpenANN::Log::kError)

namespace OpenANN
{

struct FloatingPointFormatter
{
  fpt value;
  int precision;
  FloatingPointFormatter(fpt value, int precision);
};

std::ostream& operator<<(std::ostream& os, const FloatingPointFormatter& t);

class Log 
{
public:
    enum LogLevel {
        kDisabled = 0,
        kError,
        kInfo, 
        kDebug
    };

    Log();
    virtual ~Log();

    std::ostream& get(LogLevel level);

    static std::ostream& Stream();
    static LogLevel& Level();

private:
    std::ostringstream message;
    LogLevel level;
};



class Logger
{
public:
  static bool deactivate;
  enum Target
  {
    NONE, CONSOLE, FILE, APPEND_FILE
  } target;

  std::string name;
  std::ofstream file;

  Logger(Target target, std::string name = "Logger");
  ~Logger();
  bool isActive();
};

Logger& operator<<(Logger& logger, const FloatingPointFormatter& t);

template<typename T>
Logger& operator<<(Logger& logger, const T& t)
{
  switch(logger.target)
  {
    case Logger::CONSOLE:
      std::cout << t << std::flush;
      break;
    case Logger::APPEND_FILE:
    case Logger::FILE:
      logger.file << t << std::flush;
      break;
    default: // do not log
      break;
  }
  return logger;
}

}
