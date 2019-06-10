#pragma once

#include "vec3.h"

enum class PatternList
{
    VerticalLine,
    HorizontalLine,
    Circle,
    Rectangle,
    Star,
    Cross,
    Spiral,
    Random,
    None,
    Count
};

void init_pattern_manager();

const Vec3& update_pattern_manager();

void set_pattern(const PatternList& pattern);

const PatternList& get_pattern();

const char* get_pattern_name(const PatternList& pattern);

void set_speed(const float speed);

const float get_speed();