#include "pattern_manager.h"

#include "pattern_base.h"
#include "pattern_line.h"
#include "pattern_circle.h"
#include "pattern_rectangle.h"
#include "pattern_star.h"
#include "pattern_cross.h"
#include "pattern_random.h"
#include "pattern_lissajous.h"
#include "pattern_rose.h"
#include "pattern_none.h"

#include "logger.h"

PatternBase* m_current_pattern;
PatternList m_current_pattern_id;
std::array<PatternBase*, (uint32_t)PatternList::Count> m_patterns;

const float speed = 0.5;

PatternLine         pvertical(Vec3(0.0, 1.0, 0.0), Vec3(0.0, -1.0, 0.0), "VerticalLine", speed);
PatternLine         phorizontal(Vec3(-1.0, 0.0, 0.0), Vec3(1.0, 0.0, 0.0), "HorizontalLine", speed);
PatternCircle       pcircle(Vec3(0.0, 0.0, 0.0), 0.5, 60.0);//speed);
PatternRectangle    prect(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), speed);
PatternStar         pstar(Vec3(0.0, 0.0, 0.0), 0.5, speed);
PatternCross        pcross(Vec3(0.0, 0.0, 0.0), 0.5, speed);
PatternRandom       prand(AABB({Vec3(-1.0, 1.0, 0.0), Vec3(1.0, -1.0, 0.0)}), speed);
PatternLissajous    plissa(5.0, 4.0, M_PI_2, speed);
PatternRose         prose(7, 1.0, speed);
PatternNone         pnone;

void init_pattern_manager()
{
    m_patterns[(uint32_t)PatternList::VerticalLine] = &pvertical;
    m_patterns[(uint32_t)PatternList::HorizontalLine] = &phorizontal;
    m_patterns[(uint32_t)PatternList::Circle] = &pcircle;
    m_patterns[(uint32_t)PatternList::Rectangle] = &prect;
    m_patterns[(uint32_t)PatternList::Star] = &pstar;
    m_patterns[(uint32_t)PatternList::Cross] = &pcross;
    m_patterns[(uint32_t)PatternList::Random] = &prand;
    m_patterns[(uint32_t)PatternList::Lissajous] = &plissa;
    m_patterns[(uint32_t)PatternList::Rose] = &prose;
    m_patterns[(uint32_t)PatternList::None] = &pnone;

    set_pattern(PatternList::None);

    LOG_INFO("Finished loading patterns");
}

const Vec3& update_pattern_manager()
{
    return m_current_pattern->update();
}

void set_pattern(const PatternList& pattern)
{
    m_current_pattern_id = pattern;
    m_current_pattern = m_patterns[(uint32_t)m_current_pattern_id];
}

const PatternList& get_pattern()
{
    return m_current_pattern_id;
}

const char* get_pattern_name(const PatternList& pattern)
{
    return m_patterns[(uint32_t)pattern]->get_name();
}

void set_speed(const float speed)
{
    m_current_pattern->set_speed(speed);
}

const float get_speed()
{
    return m_current_pattern->get_speed();
}