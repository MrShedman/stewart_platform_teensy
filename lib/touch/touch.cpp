#include "touch.h"

#include <Arduino.h>

#include "filter.h"

const uint8_t ur_pin = 0;
const uint8_t lr_pin = 1;
const uint8_t ll_pin = 2;
const uint8_t ul_pin = 3;

const uint8_t sense_pin = 14;

const uint8_t TOUCH_FRAME_RATE = 200;

const uint8_t axis_list_size = 32;
const uint8_t noise_list_size = 8;

typedef CircularBuffer<float, axis_list_size> PositionList;
typedef CircularBuffer<Time, axis_list_size> TimeList;
typedef CircularBuffer<float, noise_list_size> NoiseList;

struct axis_t
{
    float raw;
    float min;
    float max;
    float delta;
    float velocity;
    highPassFilter_t filter;
    PositionList position_list;
    TimeList time_list;
    NoiseList noise_list;
};

axis_t x_axis;
axis_t y_axis;

bool state = 0;
bool contact = false;
const float contact_threshold = 0.0001;

TouchList touch_list;

float read_sense_pin()
{
    return analogRead(sense_pin);
}

void update_axis(axis_t* axis)
{
    axis->raw = read_sense_pin();
    axis->time_list.push(microseconds(micros()));
    axis->position_list.push(map(axis->raw, axis->min, axis->max, -1.0, 1.0));
    axis->noise_list.push(highPassFilterApply(&axis->filter, axis->position_list.back()));
    axis->delta = 0.0;
    axis->velocity = 0.0;
    for (uint32_t i = 1; i < axis->noise_list.size(); ++i)
    {
        axis->delta += abs(axis->noise_list[i] - axis->noise_list[i - 1]);
    }

    if (axis->position_list.full())
    {
        for (uint32_t i = 1; i < axis->position_list.size(); ++i)
        {
            const float dp = axis->position_list[i] - axis->position_list[i - 1];
            const Time dt = axis->time_list[i] - axis->time_list[i - 1];
            axis->velocity += (dp / dt.asSeconds());
        }

        axis->velocity /= axis->position_list.capacity();
    }
}

void push_new_touch()
{
    Touch t;
    t.pos.x = x_axis.position_list.back();
    t.pos.y = y_axis.position_list.back();
    t.vel.x = x_axis.velocity;
    t.vel.y = y_axis.velocity;
    t.time = (x_axis.time_list.back() + y_axis.time_list.back()) * 0.5;
    t.contact = contact;
    
    touch_list.push(t);
}

void update_touch(uint32_t currentDeltaTimeUs)
{
    if (state)
    {
        update_axis(&y_axis);

        digitalWriteFast(ur_pin, LOW);
        digitalWriteFast(ll_pin, HIGH);
    }
    else
    {
        update_axis(&x_axis);

        digitalWriteFast(ur_pin, HIGH);
        digitalWriteFast(ll_pin, LOW);

        contact = (x_axis.delta * y_axis.delta) < contact_threshold;

        push_new_touch();
    }
    
    state = !state;
}

TouchList& get_touch_list()
{
    return touch_list;
}

bool is_touched()
{
    return contact;
}

void init_touch()
{
    digitalWriteFast(ul_pin, HIGH); 
    digitalWriteFast(lr_pin, LOW);

    pinMode(ul_pin, OUTPUT);
    pinMode(ur_pin, OUTPUT);
    pinMode(ll_pin, OUTPUT);
    pinMode(lr_pin, OUTPUT);

    analogReadResolution(12);
    //analogReadAveraging(32);
 
    state = 0;

    x_axis.min = 925.0;
    x_axis.max = 2990.0;
    y_axis.min = 940.0;
    y_axis.max = 3000.0;

    const float f_cut_hz = 100.0;
    highPassFilterInit(&x_axis.filter, f_cut_hz, 1.0 / TOUCH_FRAME_RATE);
    highPassFilterInit(&y_axis.filter, f_cut_hz, 1.0 / TOUCH_FRAME_RATE);
}