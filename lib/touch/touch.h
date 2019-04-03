#pragma once

#include <Arduino.h>

#include "circular_buffer.h"

extern const uint16_t TOUCH_FRAME_RATE;
extern const uint16_t CONTACT_FRAME_RATE;

struct Point
{
    float x;
    float y;
};

typedef CircularBuffer<Point, 8> PointList;

void update_touch(uint32_t currentDeltaTimeUs);

void update_contact(uint32_t currentDeltaTimeUs);

PointList& get_touch_list();

bool is_touched();

void init_touch();