#include <Arduino.h>

#include "platform.h"
#include "pattern_manager.h"
#include "controller.h"
#include "rate.h"

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