#include "platform.h"

#include "servo.h"
#include "angle.h"
#include "circular_buffer.h"

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

void calcQ()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        q[i] = translation + home_pose + rotation * platformJoint[i];
        
        l[i] = q[i] - baseJoint[i];
    }
}

void calcAlpha()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        float L = length_sq(l[i]) - (legLength * legLength) + (hornLength * hornLength);
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

bool check_for_naan()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (isnan(servo_pulse_widths[i]))
        {
            return true;
        }
    }
    return false;
}

void calcPulseWidths()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        const float width = (alpha[i] - alpha_zero[i]) * servo_rate;
        if (i % 2) 
        { 
            servo_pulse_widths[i] = servo_zeros[i] - width;
        }
        else
        {
            servo_pulse_widths[i] = servo_zeros[i] + width;
        }
    }
}

void write_servos()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        servos[i].write(servo_pulse_widths[i]);
    }
}

void init_platform()
{
    home_pose = Vec3(0.0, 0.0, initial_height);

    for (uint8_t i = 0; i < 6; i++)
    {
        baseJoint[i] = polar(degrees(baseAngles[i]), baseRadius);
        platformJoint[i] = polar(degrees(platformAngles[i]), platformRadius);
    }

    calcQ();
    calcAlphaZero();

    for (uint8_t i = 0; i < 6; i++)
    {
        servos[i].begin(servo_pins[i], servo_min, servo_max);
    }
}

bool transform_platform(const Vec3& t, const Mat33& r)
{
    translation = t;
    rotation = r;
    calcQ();
    calcAlpha();
    calcPulseWidths();
    if (check_for_naan())
    {
        return false;
    }
    else
    {
        write_servos();
        return true;
    }
}

void home_platform()
{
    transform_platform(Vec3(), Mat33());
}