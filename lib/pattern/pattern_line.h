#pragma once

#include "pattern_polyline.h"
#include "vec3.h"

class PatternLine: public PatternPolyLine<3>
{
public:

    PatternLine(const Vec3& start, const Vec3& end, const char* name, const float speed)
    :
    PatternPolyLine({start, end, start}, name, speed)
    {}
};