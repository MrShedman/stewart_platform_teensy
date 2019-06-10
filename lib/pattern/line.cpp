#include "line.h"

#include <math.h>

Line::Line()
:
m_point(),
m_start(),
m_end(),
m_total_distance_sq(0.0),
m_direction(),
m_complete(true)
{}

Line::Line(const Vec3& start, const Vec3& end)
:
m_point(start),
m_start(start),
m_end(end),
m_complete(false)
{
    m_total_distance_sq = length_sq(start - end);

    m_direction = normalise(start - end);
}

Line::Line(const Line& line)
:
m_point(line.m_point),
m_start(line.m_start),
m_end(line.m_end),
m_total_distance_sq(line.m_total_distance_sq),
m_direction(line.m_direction),
m_complete(line.m_complete)
{

}

const Vec3& Line::update(const float inc)
{
    if (m_complete)
    {
        m_point += m_direction * inc;
        if (!check_limits())
        {
            m_point = m_end;
            m_complete = true;
        }
    }
    return m_point;
}

bool Line::complete()
{
    return m_complete;
}

void Line::reset()
{
    m_point = m_start;
    m_complete = false;
}

bool Line::check_limits()
{
    const float a = distance_sq(m_start, m_point);
    const float b = distance_sq(m_end, m_point);
    return equal(a + b, m_total_distance_sq);
}

bool Line::equal(float a, float b)
{
    const float EPSILON = 0.001;
    return fabsf(a - b) < EPSILON;
}