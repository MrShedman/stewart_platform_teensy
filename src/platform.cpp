#include "platform.h"

Platform::Platform()
{
    home_pose = Vec3(0.0, 0.0, initial_height);

    for (uint8_t i = 0; i < 6; i++)
    {
        baseJoint[i] = polar(degrees(baseAngles[i]), baseRadius);
        platformJoint[i] = polar(degrees(platformAngles[i]), platformRadius);
    }

    calcQ();
    calcAlphaZero();
}

void Platform::init_servos()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        servos[i].begin(servo_pins[i], servo_min, servo_max);
    }
}

void Platform::applyTranslationAndRotation(const Vec3& t, const Mat33& r)
{
    rotation = r;
    translation = t;
    calcQ();
    calcAlpha();
    calcPulseWidths();
}

void Platform::calcQ()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        q[i] = translation + home_pose + rotation * platformJoint[i];
        
        l[i] = q[i] - baseJoint[i];
    }
}

void Platform::calcAlpha()
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

void Platform::calcAlphaZero()
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

void Platform::calcPulseWidths()
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
        
        servos[i].write(servo_pulse_widths[i]);
    }
}