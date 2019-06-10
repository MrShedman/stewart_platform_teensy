#pragma once

#include "pattern_base.h"
#include "AABB.h"
#include "line.h"

class PatternRandom : public PatternBase
{
public:

    PatternRandom(const AABB& aabb, const float speed)
    :
    PatternBase("Random", speed),
    m_aabb(aabb),
    m_line(gen_line())
    {
    }

    const Vec3& update() override
    {
        const float d = get_delta();
        m_point = m_line.update(d);

        if (m_line.complete())
        {
            m_line = gen_line();
        }

        return m_point;
    }

private:

    Vec3 gen_point()
    {
        const uint32_t res = 1000.0;
        Vec3 pt;
        pt.x = (float)random(m_aabb.min().x * res, m_aabb.max().x * res) / (float)res;
        pt.y = (float)random(m_aabb.min().y * res, m_aabb.max().y * res) / (float)res;
        return pt;
    }

    Line gen_line()
    {
        return Line(gen_point(), gen_point());
    }

    const AABB& m_aabb;
    Line m_line;
};