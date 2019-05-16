#pragma once

#include "vec3.h"

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

void init_pattern_manager();

const Vec3& update_pattern_manager();

void set_pattern(const PatternList& pattern);