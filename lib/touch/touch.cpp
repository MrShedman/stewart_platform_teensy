#include "touch.h"

#include <Arduino.h>

#include "filter.h"

highPassFilter_t filter_x;
highPassFilter_t filter_y;

const uint8_t ur_pin = 0;
const uint8_t lr_pin = 1;
const uint8_t ll_pin = 2;
const uint8_t ul_pin = 3;

const uint8_t sense_pin = 14;

IntervalTimer timer;

const uint8_t FRAME_RATE = 200;
const uint8_t CONTACT_FRAME_RATE = 30;
const uint8_t NUM_SAMPLES = 1;

volatile float px, py = 0;
volatile float dx, dy = 0;
volatile float fx, fy = 0;
volatile bool state = 0;
volatile uint8_t count = 0;
volatile uint8_t count_limit = 0;
volatile bool contact = false;

float min_x = 925.0;
float max_x = 2990.0;
float min_y = 940.0;
float max_y = 3000.0;

float read_sense_pin()
{
    float temp = 0.0;

    for (uint8_t i = 0; i < NUM_SAMPLES; ++i)
    {
        temp += analogRead(sense_pin);
    }

    return temp / (float)NUM_SAMPLES;
}

void timing_worker()
{
    if (state)
    {
        const float ay = read_sense_pin();
        const float ny = map(ay, min_y, max_y, -1.0, 1.0);
        const float nfy = highPassFilterApply(&filter_y, ny);
        dy += abs(nfy - fy);
        fy = nfy;
        py = ny;
        digitalWriteFast(ur_pin, LOW);
        digitalWriteFast(ll_pin, HIGH);
    }
    else
    {
        const float ax = read_sense_pin();
        const float nx = map(ax, min_x, max_x, -1.0, 1.0);
        const float nfx = highPassFilterApply(&filter_x, nx);
        dx += abs(nfx - fx);
        fx = nfx;
        px = nx;
        digitalWriteFast(ur_pin, HIGH);
        digitalWriteFast(ll_pin, LOW);
    }
    
    state = !state;
    count++;

    if (count > count_limit)
    {
        contact = (dx * dy * 100000.0) < 2.0;
        count = 0;
        dx = 0;
        dy = 0;
    }
}

Point get_touch_point()
{
    Point p;
    p.x = px;
    p.y = py;
    return p;
}

bool is_touched()
{
    return contact;
}

void init_touch()
{
    const float f_cut_hz = 100.0;
    highPassFilterInit(&filter_x, f_cut_hz, 1.0 / FRAME_RATE);
    highPassFilterInit(&filter_y, f_cut_hz, 1.0 / FRAME_RATE);

    digitalWriteFast(ul_pin, HIGH); 
    digitalWriteFast(lr_pin, LOW);

    pinMode(ul_pin, OUTPUT);
    pinMode(ur_pin, OUTPUT);
    pinMode(ll_pin, OUTPUT);
    pinMode(lr_pin, OUTPUT);

    analogReadResolution(12);
    //analogReadAveraging(32);
 
    count_limit = 2 * FRAME_RATE / CONTACT_FRAME_RATE;

    state = 0;
	timer.begin(timing_worker, 1000000 / (FRAME_RATE * 2));
}