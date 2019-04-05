#include "touch.h"

#include <Arduino.h>

#include "filter.h"

const uint8_t ur_pin = 0;
const uint8_t lr_pin = 1;
const uint8_t ll_pin = 2;
const uint8_t ul_pin = 3;

const uint8_t sense_pin = 14;

const uint8_t TOUCH_FRAME_RATE = 200;

struct axis_t
{
    float raw;
    float min;
    float max;
    float mapped;
    float delta;
    highPassFilter_t filter;
    CircularBuffer<float, 8> filtered_list;
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
    axis->mapped = map(axis->raw, axis->min, axis->max, -1.0, 1.0);
    axis->filtered_list.push(highPassFilterApply(&axis->filter, axis->mapped));
    axis->delta = 0.0;
    for (uint32_t i = 1; i < axis->filtered_list.size(); ++i)
    {
        axis->delta += abs(axis->filtered_list[i] - axis->filtered_list[i - 1]);
    }
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

        touch_list.push(Touch{x_axis.mapped, y_axis.mapped, contact});
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