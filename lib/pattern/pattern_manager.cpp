#include "pattern_manager.h"

#include "pattern_base.h"
#include "pattern_circle.h"
#include "pattern_rectangle.h"
#include "pattern_star.h"
#include "pattern_cross.h"
#include "pattern_spiral.h"
#include "pattern_random.h"

PatternList m_current_pattern;
std::array<PatternBase*, (uint32_t)PatternList::Count> m_patterns;
const Vec3 zero;

PatternCircle       pcircle(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
PatternRectangle    prect(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);
PatternStar         pstar(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
PatternCross        pcross(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
PatternSpiral       pspiral(Vec3(0.0, 0.0, 0.0), 0.1, 0.5, 0.05);
PatternRandom       prand(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);

void init_pattern_manager()
{
    m_patterns[(uint32_t)PatternList::Circle] = &pcircle;
    m_patterns[(uint32_t)PatternList::Rectangle] = &prect;
    m_patterns[(uint32_t)PatternList::Star] = &pstar;
    m_patterns[(uint32_t)PatternList::Cross] = &pcross;
    m_patterns[(uint32_t)PatternList::Spiral] = &pspiral;
    m_patterns[(uint32_t)PatternList::Random] = &prand;

    set_pattern(PatternList::None);
}

const Vec3& update_pattern_manager()
{
    if (m_current_pattern == PatternList::None)
    {
        return zero;
    }

    return m_patterns[(uint32_t)m_current_pattern]->update();
}

void set_pattern(const PatternList& pattern)
{
    m_current_pattern = pattern;
}