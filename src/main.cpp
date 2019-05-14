#include <Arduino.h>

#include "servo.h"
#include "filter.h"
#include "vec3.h"
#include "mat33.h"
#include "quat.h"
#include "pid.h"
#include "platform.h"
#include "rate.h"
#include "pattern_manager.h"
#include "controller.h"

Platform platform;
PatternManager pattern_manager;

Controller controller(platform, pattern_manager);

Rate rate(100.0);

void setup() 
{
    platform.init_servos();
}

void loop()
{
    controller.update();

    rate.sleep();
}