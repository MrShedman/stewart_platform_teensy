#pragma once

struct Point
{
    float x;
    float y;
};

Point get_touch_point();

bool is_touched();

void init_touch();