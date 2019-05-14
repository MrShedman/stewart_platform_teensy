#pragma once

#include "pattern_polyline.h"
#include "line.h"
#include "vec3.h"
#include "AABB.h"

class PatternRectangle : public PatternPolyLine<5>
{
public:

    PatternRectangle(const AABB& bounds, const float speed)
    :
    PatternPolyLine({
            Vec3(bounds.min().x, bounds.max().y, 0.0), Vec3(bounds.max().x, bounds.max().y, 0.0),
            Vec3(bounds.max().x, bounds.min().y, 0.0), Vec3(bounds.min().x, bounds.min().y, 0.0),
            Vec3(bounds.min().x, bounds.max().y, 0.0)
            }, speed)
    {}
};