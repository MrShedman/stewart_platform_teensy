#pragma once

#include <Arduino.h>

#include "vec3.h"
#include "servo.h"

class Platform 
{
public:

    Vec3 translation, rotation, home_pose;
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
    const float servo_rate = 1600/PI;
    const float servo_zeros[6] = {1475.0, 1470.0, 1490.0, 1480.0, 1460.0, 1490.0};
    const uint8_t servo_pins[6] = {5, 6, 9, 20, 21, 22};

    float servo_pulse_widths[6];
    Servo servos[6];
    // REAL ANGLES

    //angles small platform, need to invert the servo horns
    const float baseAngles[6] = {314.9, 345.1, 74.9, 105.1, 194.9, 225.1};

    const float platformAngles[6] = {322.9, 337.1, 82.9, 97.1, 202.9, 217.1};

    const float beta[6] = {-8 * PI / 3, PI / 3, 0, -PI, -4 * PI / 3, -7 * PI / 3};

    Platform()
    {
        home_pose = Vec3(0.0, 0.0, initial_height);

        for (uint8_t i = 0; i < 6; i++)
        {
            float mx = baseRadius * cosf(radians(baseAngles[i]));
            float my = baseRadius * sinf(radians(baseAngles[i]));
            baseJoint[i] = Vec3(mx, my, 0);
        }

        for (uint8_t i = 0; i < 6; i++)
        {
            float mx = platformRadius * cosf(radians(platformAngles[i]));
            float my = platformRadius * sinf(radians(platformAngles[i]));
            platformJoint[i] = Vec3(mx, my, 0);
        }

        calcQ();
        calcAlphaZero();
    }

    void init_servos()
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            servos[i].begin(servo_pins[i], servo_min, servo_max);
        }
    }

    void applyTranslationAndRotation(const Vec3& t, const Vec3& r)
    {
        rotation = r;
        translation = t;
        calcQ();
        calcAlpha();
        calcPulseWidths();
    }

    void calcQ()
    {
        const float cx = cosf(rotation.x);
        const float sx = sinf(rotation.x);
        const float cy = cosf(rotation.y);
        const float sy = sinf(rotation.y); 
        const float cz = cosf(rotation.z);
        const float sz = sinf(rotation.z);

        for (uint8_t i = 0; i < 6; i++)
        {
            // rotation
            q[i].x = cz * cy * platformJoint[i].x +
                    (-sz * cx + cz * sy * sx) * platformJoint[i].y +
                    (sz * sx + cz * sy * cx) * platformJoint[i].z;

            q[i].y = sz * cy * platformJoint[i].x +
                    (cz * cx + sz * sy * sx) * platformJoint[i].y +
                    (-cz * sx + sz * sy * cx) * platformJoint[i].z;

            q[i].z = -sy * platformJoint[i].x +
                    cy * sx * platformJoint[i].y +
                    cy * cx * platformJoint[i].z;

            // translation
            q[i] = translation + home_pose;
            l[i] = q[i] - baseJoint[i];
        }
    }

    void calcAlpha()
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            float l_sq = dot(l[i], l[i]);
            float L = l_sq - (legLength * legLength) + (hornLength * hornLength);
            float M = 2.0 * hornLength * (q[i].z - baseJoint[i].z);
            float N = 2.0 * hornLength * (cosf(beta[i]) * (q[i].x - baseJoint[i].x) + sinf(beta[i]) * (q[i].y - baseJoint[i].y));
            alpha[i] = asinf(L / sqrtf(M * M + N * N)) - atan2f(N, M);

            A[i] = Vec3(hornLength * cosf(alpha[i]) * cosf(beta[i]) + baseJoint[i].x,
                        hornLength * cosf(alpha[i]) * sinf(beta[i]) + baseJoint[i].y,
                        hornLength * sinf(alpha[i]) + baseJoint[i].z);
        }
    }

    void calcAlphaZero()
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            float xqxb = (q[i].x - baseJoint[i].x);
            float yqyb = (q[i].y - baseJoint[i].y);
            float h0 = sqrtf((legLength * legLength) + (hornLength * hornLength) - (xqxb * xqxb) - (yqyb * yqyb)) - q[i].z;

            float L0 = 2 * hornLength * hornLength;
            float M0 = 2 * hornLength * (h0 + q[i].z);
            float N0 = 2.0 * hornLength * (cosf(beta[i]) * (q[i].x - baseJoint[i].x) + sinf(beta[i]) * (q[i].y - baseJoint[i].y));

            alpha_zero[i] = asinf(L0 / sqrtf(M0 * M0 + N0 * N0)) - atan2f(N0, M0);
        } 
    }

    void calcPulseWidths()
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            if (i % 2) 
            { 
                servo_pulse_widths[i] = servo_zeros[i] - (alpha[i] -  alpha_zero[i]) * servo_rate;
            }
            else
            {
                servo_pulse_widths[i] = servo_zeros[i] + (alpha[i] -  alpha_zero[i]) * servo_rate;
            }            
        }
    }
};