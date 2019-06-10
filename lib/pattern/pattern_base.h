#pragma once

#include "clock.h"
#include "vec3.h"

class PatternBase
{
public:

    PatternBase(const char* name, const float speed)
    :
    m_name(name),
    m_speed(speed)
    {}

    virtual const Vec3& update() = 0;

    const char* get_name() const
    {
        return m_name;
    }

    void set_speed(const float speed)
    {
        m_speed = speed;
    }

    const float get_speed() const
    {
        return m_speed;
    }

protected:

    const float get_delta()
    {
        return m_speed * m_clock.restart().asSeconds();
    }

    const char* m_name;
    float m_speed;
    Vec3 m_point;
    Clock m_clock;
};