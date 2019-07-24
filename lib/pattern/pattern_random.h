#pragma once

#include "pattern_base.h"
#include "AABB.h"
#include "line.h"

#include "logger.h"

class PatternRandom : public PatternBase
{
public:

    PatternRandom(const AABB& aabb, const float speed)
    :
    PatternBase("Random", speed),
    m_aabb(aabb)
    {
        //srand(analogRead(0));
        pt1 = gen_point();
        pt2 = gen_point();
        m_line = Line(pt1, pt2);

        //LOG_INFO("AABB: %.3f, %.3f, %.3f", m_aabb.max().x, m_aabb.max().y, m_aabb.max().z);
        //LOG_INFO("AABB: %.3f, %.3f, %.3f", m_aabb.min().x, m_aabb.min().y, m_aabb.min().z);
    }

    const Vec3& update() override
    {
        /*for (int i = 0; i < 1000; ++i)
        {
            pt1 = gen_point();
            //LOG_INFO("%.3f, %.3f, %.3f", pt1.x, pt1.y, pt1.z);
            delayMicroseconds(5000);
        }
        while(true);*/
        const float d = get_delta();
        m_point = m_line.update(d);

        if (m_line.complete())
        {
            pt1 = pt2;
            pt2 = gen_point();
            /*if (pt2.x == 0.0 and pt2.y == 0.0 and pt2.z == 0.0)
            {
                while(true);
            }*/
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
        //pt.x = (float)random(m_aabb.min().x * res, m_aabb.max().x * res) / (float)res;
        //pt.y = (float)random(m_aabb.min().y * res, m_aabb.max().y * res) / (float)res;
        ///LOG_INFO("%.3f, %.3f, %.3f", m_aabb.max().x, m_aabb.max().y, m_aabb.max().z);
        //LOG_INFO("%.3f, %.3f, %.3f", pt.x, pt.y, pt.z);
        return pt;
    }

    Vec3 pt1;
    Vec3 pt2;

    const AABB m_aabb;
    Line m_line;
};