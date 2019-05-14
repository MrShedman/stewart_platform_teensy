#pragma once

#include "vec3.h"

class Line
{
public:

    Line();

    Line(const Vec3& start, const Vec3& end);

    Line(const Line& line);

    const Vec3& update(const float inc);

    bool complete();

    void reset();

private:

    bool check_limits();

    Vec3 m_point;
    Vec3 m_start;
    Vec3 m_end;
    float m_total_distance_sq;
    Vec3 m_direction;
    bool m_complete;
};