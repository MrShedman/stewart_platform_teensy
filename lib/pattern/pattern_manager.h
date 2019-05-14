#pragma once

#include "pattern_base.h"
#include "pattern_circle.h"
#include "pattern_rectangle.h"
#include "pattern_star.h"
#include "pattern_cross.h"
#include "pattern_spiral.h"
#include "pattern_random.h"

#include <array>

enum class PatternList
{
    Circle,
    Rectangle,
    Star,
    Cross,
    Spiral,
    Random,
    Count,
    None
};

class PatternManager
{
public:

    PatternManager()
    {
        m_patterns[(uint32_t)PatternList::Circle] = new PatternCircle(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
        m_patterns[(uint32_t)PatternList::Rectangle] = new PatternRectangle(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);
        m_patterns[(uint32_t)PatternList::Star] = new PatternStar(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
        m_patterns[(uint32_t)PatternList::Cross] = new PatternCross(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
        m_patterns[(uint32_t)PatternList::Spiral] = new PatternSpiral(Vec3(0.0, 0.0, 0.0), 0.1, 0.5, 0.05);
        m_patterns[(uint32_t)PatternList::Random] = new PatternRandom(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);

        setPattern(PatternList::None);
    }

    const Vec3 update()
    {
        if (m_current_pattern == PatternList::None)
        {
            return Vec3();
        }

        return m_patterns[(uint32_t)m_current_pattern]->update();
    }

    void setPattern(const PatternList& pattern)
    {
        m_current_pattern = pattern;
    }

private:

    PatternList m_current_pattern;
    std::array<PatternBase*, (uint32_t)PatternList::Count> m_patterns;
};