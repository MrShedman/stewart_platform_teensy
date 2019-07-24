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
    m_aabb(aabb)
    {
        pt1 = gen_point();
        pt2 = gen_point();
        m_line = Line(pt1, pt2);
    }

    const Vec3& update() override
    {
        const float d = get_delta();
        m_point = m_line.update(d);

        if (m_line.complete())
        {
            pt1 = pt2;
            pt2 = gen_point();
            m_line = Line(pt1, pt2);
        }

        return m_point;
    }

private:

    Vec3 gen_point()
    {
        const uint32_t res = 1000.0;
        Vec3 pt;

        pt.x = map((float)random(0, res), 0.0, (float)res, m_aabb.min().x, m_aabb.max().x);
        pt.y = map((float)random(0, res), 0.0, (float)res, m_aabb.min().y, m_aabb.max().y);
        return pt;
    }

    Vec3 pt1;
    Vec3 pt2;

    const AABB m_aabb;
    Line m_line;
};