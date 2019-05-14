#pragma once

#include "pattern_base.h"
#include "angle.h"

class PatternSpiral : public PatternBase
{
public:

    PatternSpiral(const Vec3& origin, const float min_radius, const float max_radius, const float speed)
    :
    PatternBase(speed),
    m_angle(degrees(90.0)),
    m_radius(m_min_radius),
    m_direction(1.0),
    m_origin(origin),
    m_min_radius(min_radius),
    m_max_radius(max_radius)
    {
        set_angle();
    }

    virtual const Vec3& update() override
    {
        const float d = get_delta();
        m_angle += degrees(d);
        m_radius += d * m_direction;
        if (m_radius > m_max_radius)
        {
            m_radius = m_max_radius;
            m_direction *= -1.0;
        }
        else if (m_radius < m_min_radius)
        {
            m_radius = m_min_radius;
            m_direction *= -1.0;
        }

        set_angle();
        return m_point;
    }

protected:

    void set_angle()
    {
        m_point = polar(m_angle, m_radius);
    }

    Angle m_angle;
    float m_radius;
    float m_direction;
    const Vec3 m_origin;
    const float m_min_radius;
    const float m_max_radius;
};