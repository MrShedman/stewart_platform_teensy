#pragma once

#include <Arduino.h>

#include "circular_buffer.h"
#include "time.h"
#include "vec3.h"

extern const float TOUCH_FRAME_RATE;

struct Touch
{
    Vec3 pos;
    Vec3 vel;
    Time time;
    bool contact;
};

typedef CircularBuffer<Touch, 8> TouchList;

void update_touch(const Time& currentDeltaTimeUs);

TouchList& get_touch_list();

bool is_touched();

void init_touch();