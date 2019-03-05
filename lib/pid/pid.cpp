#include "pid.h"

void PID::init(const pidProfile_t& profile)
{
    p_gain = profile.kp;
    i_gain = profile.ki;
    d_gain = profile.kd;
    max_i = profile.max_I;
    max_output = profile.max_Out;

    if (profile.dterm_lpf_hz > 0.0f && profile.lpf_dT > 0.0f)
    {
        pt1FilterInit(&dterm_filter, profile.dterm_lpf_hz, profile.lpf_dT);
    }

    reset();
}

float PID::update(float input, float setpoint)
{
	const float error_temp = input - setpoint;

	float kp = p_gain;
	float ki = i_gain;
	float kd = d_gain;

	i_mem += ki * error_temp;
	i_mem = constrain(i_mem, -max_i, max_i);

	float dterm = kd * (error_temp - last_d_error);
	last_d_error = error_temp;

	dterm = pt1FilterApply(&dterm_filter, dterm);

	output = kp * error_temp + i_mem + dterm;
	output = constrain(output, -max_output, max_output);

    return get_output();
}

void PID::reset()
{
    i_mem = 0;
    last_d_error = 0;
}

float PID::get_output() const
{
    return output;
}