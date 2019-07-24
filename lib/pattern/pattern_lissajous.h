#pragma once

#include "pattern_base.h"
#include "angle.h"

class PatternLissajous : public PatternBase
{
public:

    PatternLissajous(const float a, const float b, const float d, const float speed)
    :
    PatternBase("Lissajous", speed),
    a(a),
    b(b),
    d(d)
    {}

    virtual const Vec3& update() override
    {
        t += get_delta();
        m_point = calc(t);
        return m_point;
    }

protected:

    Vec3 calc(float dt)
    {
        float x = sinf(a * dt + d);
        float y = sinf(b * dt);
        return Vec3(x, y, 0.0);
    }

    float a;
    float b;
    float d;
    float t;
};