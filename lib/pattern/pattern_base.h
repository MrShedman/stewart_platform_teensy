#pragma once

#include "time.h"
#include "vec3.h"

class PatternBase
{
public:

    PatternBase(const float speed)
    :
    m_speed(speed)
    {}

    virtual const Vec3& update() = 0;

protected:

    const float get_delta()
    {
        float dt = (Time::now() - m_last_update).asSeconds();
        m_last_update = Time::now();
        return m_speed * dt;
    }

    const float m_speed;
    Vec3 m_point;
    Time m_last_update;
};