#pragma once

#include "pattern_base.h"
#include "angle.h"

class PatternCircle : public PatternBase
{
public:

    PatternCircle(const Vec3& origin, const float radius, const float speed)
    :
    PatternBase(speed),
    m_angle(degrees(90.0)),
    m_origin(origin),
    m_radius(radius)
    {
        set_angle();
    }

    virtual const Vec3& update() override
    {
        const float d = get_delta();
        m_angle += degrees(d);
        set_angle();
        return m_point;
    }

protected:

    void set_angle()
    {
        m_point = polar(m_angle, m_radius);
    }

    Angle m_angle;
    const Vec3 m_origin;
    float m_radius;
};