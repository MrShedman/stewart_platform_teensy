#pragma once

#include <Print.h>

enum Level
{
  Debug,
  Info,
  Warn,
  Error,
  Fatal,

  Count
};

struct LogLocation
{
    bool initialized_;
    Level level_;
};

template<class T>   inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>          inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 3); return obj; }

class StreamPrint : public Print
{
public:

    StreamPrint()
    :
    id(0)
    {
        memset(buffer, 0, sz);
    }

    size_t write(uint8_t b) override
    {
        buffer[id] = b;
        id++;
        if (id < sz)
        {
            return 1;
        }
        else
        {
            id = 0;
            return -1;
        }
    }

    operator const char*() const
    {
        return reinterpret_cast<const char*>(&buffer[0]);
    }

    static const uint8_t sz = 128;
    uint8_t id = 0;
    uint8_t buffer[sz];
};

class LoggerBase
{
public:
    virtual void print(Level level, const char* file, int line, const char* fmt, ...) = 0;
    virtual void print(Level level, const char* str, const char* file, int line) = 0;
};

extern LoggerBase* logger_ptr;

void setLogger(LoggerBase* log_ptr);

void initializeLogLocation(LogLocation* loc, Level level);

void setLogLocationLevel(LogLocation* loc, Level level);

const char* getLevelString(Level level);

#define COND_LIKELY(x)       __builtin_expect((x),1)
#define COND_UNLIKELY(x)     __builtin_expect((x),0)

// These allow you to compile-out everything below a certain severity level if necessary
#define LOG_SEVERITY_DEBUG 0
#define LOG_SEVERITY_INFO  1
#define LOG_SEVERITY_WARN  2
#define LOG_SEVERITY_ERROR 3
#define LOG_SEVERITY_FATAL 4
#define LOG_SEVERITY_NONE  5

/**
 * \def LOG_MIN_SEVERITY
 *
 * Define LOG_MIN_SEVERITY=LOG_SEVERITY_[DEBUG|INFO|WARN|ERROR|FATAL] in your build options to compile out anything below that severity
 */
#ifndef LOG_MIN_SEVERITY
#define LOG_MIN_SEVERITY LOG_SEVERITY_DEBUG
#endif

#define LOG_DEFINE_LOCATION(cond, level) \
  static LogLocation loc = {false, Level::Count}; /* Initialized at compile-time */ \
  if (COND_UNLIKELY(!loc.initialized_)) \
  { \
    initializeLogLocation(&loc, level); \
  } \
  if (COND_UNLIKELY(loc.level_ != level)) \
  { \
    setLogLocationLevel(&loc, level); \
  } \
  bool enabled = cond;


#define LOG_PRINT_AT_LOCATION(...) \
    if (logger_ptr) \
        logger_ptr->print(loc.level_, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_PRINT_STREAM_AT_LOCATION(args) \
    if (logger_ptr) \
    { \
        StreamPrint sp; \
        sp << args; \
        logger_ptr->print(loc.level_, sp, __FILE__, __LINE__); \
    }

#define LOG_COND(cond, level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(cond, level); \
    \
    if (COND_UNLIKELY(enabled)) \
    { \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG_STREAM_COND(cond, level, args) \
  do \
  { \
    LOG_DEFINE_LOCATION(cond, level); \
    \
    if (COND_UNLIKELY(enabled)) \
    { \
      LOG_PRINT_STREAM_AT_LOCATION(args); \
    } \
  } while(0)

#define LOG_ONCE(level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static bool hit = false; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(!hit)) \
    { \
      hit = true; \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG_STREAM_ONCE(level, args) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static bool hit = false; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(!hit)) \
    { \
      hit = true; \
      LOG_PRINT_STREAM_AT_LOCATION(args); \
    } \
  } while(0)

#define LOG_THROTTLE(rate, level, ...) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static float last_hit = 0.0f; \
    float now = (float)micros() / 1000000.0f; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(last_hit + (1.0f/(float)rate) <= now)) \
    { \
      last_hit = now; \
      LOG_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)

#define LOG_STREAM_THROTTLE(rate, level, args) \
  do \
  { \
    LOG_DEFINE_LOCATION(true, level); \
    static float last_hit = 0.0f; \
    float now = (float)micros() / 1000000.0f; \
    if (COND_UNLIKELY(enabled) && COND_UNLIKELY(last_hit + (1.0f/(float)rate) <= now)) \
    { \
      last_hit = now; \
      LOG_PRINT_STREAM_AT_LOCATION(args); \
    } \
  } while(0)

#define LOG(level, ...) LOG_COND(true, level, __VA_ARGS__)

#define LOG_STREAM(level, args) LOG_STREAM_COND(true, level, args)

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_DEBUG)
#define LOG_DEBUG(...)
#define LOG_DEBUG_COND(cond, ...)
#define LOG_DEBUG_ONCE(...)
#define LOG_DEBUG_THROTTLE(rate, ...)
#define LOG_STREAM_DEBUG(args)
#define LOG_STREAM_DEBUG_COND(cond, args)
#define LOG_STREAM_DEBUG_ONCE(args)
#define LOG_STREAM_DEBUG_THROTTLE(rate, args)
#else
#define LOG_DEBUG(...) LOG(Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_COND(cond, ...) LOG_COND(cond, Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_ONCE(...) LOG_ONCE(Level::Debug, __VA_ARGS__)
#define LOG_DEBUG_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Debug, __VA_ARGS__)
#define LOG_STREAM_DEBUG(args) LOG_STREAM(Level::Debug, args)
#define LOG_STREAM_DEBUG_COND(cond, args) LOG_STREAM_COND(cond, Level::Debug, args)
#define LOG_STREAM_DEBUG_ONCE(args) LOG_STREAM_ONCE(Level::Debug, args)
#define LOG_STREAM_DEBUG_THROTTLE(rate, args) LOG_STREAM_THROTTLE(rate, Level::Debug, args)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_INFO)
#define LOG_INFO(...)
#define LOG_INFO_COND(cond, ...)
#define LOG_INFO_ONCE(...)
#define LOG_INFO_THROTTLE(rate, ...)
#define LOG_STREAM_INFO(args)
#define LOG_STREAM_INFO_COND(cond, args)
#define LOG_STREAM_INFO_ONCE(args)
#define LOG_STREAM_INFO_THROTTLE(rate, args)
#else
#define LOG_INFO(...) LOG(Level::Info, __VA_ARGS__)
#define LOG_INFO_COND(cond, ...) LOG_COND(cond, Level::Info, __VA_ARGS__)
#define LOG_INFO_ONCE(...) LOG_ONCE(Level::Info, __VA_ARGS__)
#define LOG_INFO_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Info, __VA_ARGS__)
#define LOG_STREAM_INFO(args) LOG_STREAM(Level::Info, args)
#define LOG_STREAM_INFO_COND(cond, args) LOG_STREAM_COND(cond, Level::Info, args)
#define LOG_STREAM_INFO_ONCE(args) LOG_STREAM_ONCE(Level::Info, args)
#define LOG_STREAM_INFO_THROTTLE(rate, args) LOG_STREAM_THROTTLE(rate, Level::Info, args)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_WARN)
#define LOG_WARN(...)
#define LOG_WARN_COND(cond, ...)
#define LOG_WARN_ONCE(...)
#define LOG_WARN_THROTTLE(rate, ...)
#define LOG_STREAM_WARN(args)
#define LOG_STREAM_WARN_COND(cond, args)
#define LOG_STREAM_WARN_ONCE(args)
#define LOG_STREAM_WARN_THROTTLE(rate, args)
#else
#define LOG_WARN(...) LOG(Level::Warn, __VA_ARGS__)
#define LOG_WARN_COND(cond, ...) LOG_COND(cond, Level::Warn, __VA_ARGS__)
#define LOG_WARN_ONCE(...) LOG_ONCE(Level::Warn, __VA_ARGS__)
#define LOG_WARN_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Warn, __VA_ARGS__)
#define LOG_STREAM_WARN(args) LOG_STREAM(Level::Warn, args)
#define LOG_STREAM_WARN_COND(cond, args) LOG_STREAM_COND(cond, Level::Warn, args)
#define LOG_STREAM_WARN_ONCE(args) LOG_STREAM_ONCE(Level::Warn, args)
#define LOG_STREAM_WARN_THROTTLE(rate, args) LOG_STREAM_THROTTLE(rate, Level::Warn, args)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_ERROR)
#define LOG_ERROR(...)
#define LOG_ERROR_COND(cond, ...)
#define LOG_ERROR_ONCE(...)
#define LOG_ERROR_THROTTLE(rate, ...)
#define LOG_STREAM_ERROR(args)
#define LOG_STREAM_ERROR_COND(cond, args)
#define LOG_STREAM_ERROR_ONCE(args)
#define LOG_STREAM_ERROR_THROTTLE(rate, args)
#else
#define LOG_ERROR(...) LOG(Level::Error, __VA_ARGS__)
#define LOG_ERROR_COND(cond, ...) LOG_COND(cond, Level::Error, __VA_ARGS__)
#define LOG_ERROR_ONCE(...) LOG_ONCE(Level::Error, __VA_ARGS__)
#define LOG_ERROR_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Error, __VA_ARGS__)
#define LOG_STREAM_ERROR(args) LOG_STREAM(Level::Error, args)
#define LOG_STREAM_ERROR_COND(cond, args) LOG_STREAM_COND(cond, Level::Error, args)
#define LOG_STREAM_ERROR_ONCE(args) LOG_STREAM_ONCE(Level::Error, args)
#define LOG_STREAM_ERROR_THROTTLE(rate, args) LOG_STREAM_THROTTLE(rate, Level::Error, args)
#endif

#if (LOG_MIN_SEVERITY > LOG_SEVERITY_FATAL)
#define LOG_FATAL(...)
#define LOG_FATAL_COND(cond, ...)
#define LOG_FATAL_ONCE(...)
#define LOG_FATAL_THROTTLE(rate, ...)
#define LOG_STREAM_FATAL(args)
#define LOG_STREAM_FATAL_COND(cond, args)
#define LOG_STREAM_FATAL_ONCE(args)
#define LOG_STREAM_FATAL_THROTTLE(rate, args)
#else
#define LOG_FATAL(...) LOG(Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_COND(cond, ...) LOG_COND(cond, Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_ONCE(...) LOG_ONCE(Level::Fatal, __VA_ARGS__)
#define LOG_FATAL_THROTTLE(rate, ...) LOG_THROTTLE(rate, Level::Fatal, __VA_ARGS__)
#define LOG_STREAM_FATAL(args) LOG_STREAM(Level::Fatal, args)
#define LOG_STREAM_FATAL_COND(cond, args) LOG_STREAM_COND(cond, Level::Fatal, args)
#define LOG_STREAM_FATAL_ONCE(args) LOG_STREAM_ONCE(Level::Fatal, args)
#define LOG_STREAM_FATAL_THROTTLE(rate, args) LOG_STREAM_THROTTLE(rate, Level::Fatal, args)
#endif