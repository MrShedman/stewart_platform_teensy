#pragma once

#include "pattern_polyline.h"
#include "line.h"
#include "vec3.h"
#include "angle.h"

class PatternCross : public PatternPolyLine<9>
{
public:

    PatternCross(const Vec3& origin, const float radius, const float speed)
    :
    PatternPolyLine({
            polar(degrees(0),   0.0)    + origin,
            polar(degrees(0),   radius) + origin,
            polar(degrees(0),   0.0)    + origin,
            polar(degrees(90),  radius) + origin,
            polar(degrees(0),   0.0)    + origin,
            polar(degrees(180), radius) + origin,
            polar(degrees(0),   0.0)    + origin,
            polar(degrees(270), radius) + origin,
            polar(degrees(0),   0.0)    + origin
            }, "Cross", speed)
    {}
};