#pragma once

#include <Arduino.h>

#include "vec3.h"
#include "mat33.h"

void init_platform();

bool transform_platform(const Vec3& t, const Mat33& r);

void home_platform();