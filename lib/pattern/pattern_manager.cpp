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

void init_pattern_manager()
{
    m_patterns[(uint32_t)PatternList::Circle] = new PatternCircle(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
    m_patterns[(uint32_t)PatternList::Rectangle] = new PatternRectangle(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);
    m_patterns[(uint32_t)PatternList::Star] = new PatternStar(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
    m_patterns[(uint32_t)PatternList::Cross] = new PatternCross(Vec3(0.0, 0.0, 0.0), 0.5, 0.05);
    m_patterns[(uint32_t)PatternList::Spiral] = new PatternSpiral(Vec3(0.0, 0.0, 0.0), 0.1, 0.5, 0.05);
    m_patterns[(uint32_t)PatternList::Random] = new PatternRandom(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), 0.05);

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