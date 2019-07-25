#pragma once

#include <Arduino.h>
#include <Stream.h>

#include "logger.h"

class LoggerSerial : public LoggerBase
{
public:

    LoggerSerial(Stream* stream = &Serial)
    :
    stream_ptr(stream)
    {
        setLogger(this);
    }

    void print(Level level, const char* file, int line, const char* fmt, ...) override
    {
        const float now = (float)millis() / 1000.0f;
        stream_ptr->printf("[%s] [%.3f]: ", getLevelString(level), now);

        va_list ap;
        va_start(ap, fmt);

        vdprintf((int)stream_ptr, fmt, ap);

        stream_ptr->println();
    }

    void print(Level level, const char* str, const char* file, int line) override
    {
        print(level, file, line, "%s", str);
    }
private:
    Stream* stream_ptr;
};
