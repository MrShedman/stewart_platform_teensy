#pragma once

#include <Arduino.h>

#include "vec3.h"
#include "mat33.h"
#include "servo.h"
#include "angle.h"

class Platform 
{
public:

    Vec3 translation, home_pose;
    Mat33 rotation;

    Vec3 baseJoint[6];
    Vec3 platformJoint[6];
    Vec3 q[6];
    Vec3 l[6];
    Vec3 A[6];
    float alpha[6];
    float alpha_zero[6];

    const float initial_height = 120.0;
    const float baseRadius = 65.43;
    const float platformRadius = 76.35;
    const float hornLength = 36.0;
    const float legLength = 125.0;

    const float servo_min = 800.0;
    const float servo_max = 2200.0;
    const float servo_rate = 400/M_PI;
    const float servo_zeros[6] = {1475.0, 1470.0, 1490.0, 1480.0, 1460.0, 1490.0};
    const uint8_t servo_pins[6] = {5, 6, 9, 20, 21, 22};

    float servo_pulse_widths[6];
    Servo servos[6];
    // REAL ANGLES

    //angles small platform, need to invert the servo horns
    const float baseAngles[6] = {314.9, 345.1, 74.9, 105.1, 194.9, 225.1};

    const float platformAngles[6] = {322.9, 337.1, 82.9, 97.1, 202.9, 217.1};

    const float beta[6] = {-8 * M_PI / 3, M_PI / 3, 0, -M_PI, -4 * M_PI / 3, -7 * M_PI / 3};

    Platform();

    void init_servos();

    void applyTranslationAndRotation(const Vec3& t, const Mat33& r);

    void calcQ();

    void calcAlpha();

    void calcAlphaZero();

    void calcPulseWidths();
};