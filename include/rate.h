#pragma once

#include <Arduino.h>

#include "time.h"

class Rate
{
public:

    Rate(const float hz)
    :
    m_loop_time(seconds(1.0 / hz)),
    m_loop_start_time(Time::now())
    {

    }

    void sleep()
    {
        while (Time::now() - m_loop_start_time < m_loop_time);

        m_loop_start_time = Time::now();
    }

private:

    const Time m_loop_time;
    Time m_loop_start_time;
};