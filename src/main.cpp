#include <Arduino.h>

#include "servo.h"
#include "filter.h"
#include "vec3.h"
#include "mat33.h"
#include "quat.h"
#include "pid.h"
#include "platform.h"
#include "rate.h"

#include "logger_serial.h"

LoggerSerial logger;

Platform platform;

Rate rate(100.0);

void setup() 
{
    PID p;

    p.setkp(1.0).setki(0.1).setkd(2.0);
    p.setGains(1.0, 0.1, 2.0);

    platform.init_servos();

    LOG_INFO("Init Succesfully");
}

uint32_t i = 0;

void loop()
{
    float t = 10.0;

    float x = t * sinf(i * 0.01);
    float y = t * cosf(i * 0.01);
    float z = t * sinf(i * 0.001);

    float a = Angle::fromDegrees(15).asRadians();

    float r = a * sinf(i * 0.01);
    float p = a * cosf(i * 0.01);

    Mat33 rot;
    rot.setRPY(Vec3(r, p, 0.0));
    Vec3 pos(x, y, z);
    platform.applyTranslationAndRotation(pos, rot);
    i++;

    LOG_INFO("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f", platform.servo_pulse_widths[0],
                                                platform.servo_pulse_widths[1],
                                                platform.servo_pulse_widths[2],
                                                platform.servo_pulse_widths[3],
                                                platform.servo_pulse_widths[4],
                                                platform.servo_pulse_widths[5]);
    rate.sleep();
}