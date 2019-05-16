#pragma once

#include <Arduino.h>

#include "time.h"

void init_control();

void update_control(const Time& currentTime);