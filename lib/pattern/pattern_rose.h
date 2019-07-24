#pragma once

#include "pattern_base.h"
#include "angle.h"

class PatternRose : public PatternBase
{
public:

    PatternRose(const uint8_t n, const float r, const float speed)
    :
    PatternBase("Rose", speed),
    n(n),
    r(r)
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
        float x = r * cosf((float)n * dt) * cosf(dt);
        float y = r * cosf((float)n * dt) * sinf(dt);

        return Vec3(x, y, 0.0);
    }

    uint8_t n;
    float r;
    float t;
};