#pragma once

#include <Arduino.h>

#include "time.h"
#include "vec3.h"

void init_control();

void update_control(const Time& currentTime);

void reset_errors();

bool has_errors();

void set_target_pos(const Vec3& pos);