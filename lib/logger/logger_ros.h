#pragma once

#include "ros.h"
#include "logger.h"

namespace ros
{

template<uint32_t BUFFER_SIZE = 128>
class Logger_ : public LoggerBase
{
public:

    explicit Logger_(NodeHandle* nh_ptr)
    :
    nh_ptr(nh_ptr)
    {
        memset(log_buffer, 0, BUFFER_SIZE);
        setLogger(this);
    }

    void print(Level level, const char* file, int line, const char* fmt, ...) override
    {
        if (nh_ptr == nullptr) return;

        memset(log_buffer, 0, BUFFER_SIZE);
        va_list args;
        va_start(args, fmt);

        const int32_t result = vsnprintf(log_buffer, BUFFER_SIZE, fmt, args);

        va_end(args);

        if (result > 0 && result < (int32_t)BUFFER_SIZE)
        {
            print(level, log_buffer, file, line);
        }
        else
        {
            return nh_ptr->logerror(log_error_msg);
        }
    }

    void print(Level level, const char* str, const char* file, int line) override
    {
        switch (level)
        {
            case Debug: return nh_ptr->logdebug(str);
            case Info:  return nh_ptr->loginfo(str);
            case Warn:  return nh_ptr->logwarn(str);
            case Error: return nh_ptr->logerror(str);
            case Fatal: return nh_ptr->logfatal(str);
            default:    return nh_ptr->logerror(log_error_msg);
        }
    }

private:

    char log_buffer[BUFFER_SIZE];

    const char* log_error_msg = "Error writing log!";

    NodeHandle* nh_ptr;
};

typedef Logger_<> Logger;

}