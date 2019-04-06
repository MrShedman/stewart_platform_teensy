#pragma once

#include <Arduino.h>

#include "time.h"

class Rate
{
public:

    Rate(const float hz)
    :
    m_loop_time(seconds(1e6 / m_rate)),
    m_loop_start_time(microseconds(micros())
    {

    }

    void sleep()
    {
        while (microseconds(micros()) - m_loop_start_time < m_loop_time);

        m_loop_start_time = microseconds(micros());
    }

private:

    const Time m_loop_time;
    Time m_loop_start_time;
};