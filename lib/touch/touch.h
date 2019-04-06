#pragma once

#include <Arduino.h>

#include "circular_buffer.h"
#include "time.h"

extern const uint8_t TOUCH_FRAME_RATE;

struct Touch
{
    float x;
    float y;
    float vx;
    float vy;
    Time time;
    bool contact;
};

typedef CircularBuffer<Touch, 8> TouchList;

void update_touch(uint32_t currentDeltaTimeUs);

TouchList& get_touch_list();

bool is_touched();

void init_touch();