#pragma once

#include "pattern_polyline.h"
#include "line.h"
#include "vec3.h"
#include "angle.h"

class PatternStar : public PatternPolyLine<6>
{
public:

    PatternStar(const Vec3& origin, const float radius, const float speed)
    :
    PatternPolyLine({
            polar(degrees(0),   radius) + origin,
            polar(degrees(144), radius) + origin,
            polar(degrees(288), radius) + origin,
            polar(degrees(72),  radius) + origin,
            polar(degrees(216), radius) + origin,
            polar(degrees(0),   radius) + origin
            }, "Star", speed)
    {}
};