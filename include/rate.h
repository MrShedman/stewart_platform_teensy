#pragma once

#include <Arduino.h>

class Rate
{
public:

    Rate(const float hz)
    :
    m_rate(hz),
    m_loop_time_us(1e6 / m_rate),
    m_loop_start_time(micros())
    {

    }

    void sleep()
    {
        while (micros() - m_loop_start_time < m_loop_time_us);

        m_loop_start_time = micros();
    }

private:

    const float m_rate;
    const uint32_t m_loop_time_us;
    uint32_t m_loop_start_time;
};