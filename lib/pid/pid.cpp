#include "pid.h"

namespace
{
    float constrain(float x, float min, float max)
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
}

void PID::init(const pidProfile_t& profile)
{
    prof = profile;
    use_dterm_filter = false;

    if (prof.dterm_lpf_hz > 0.0 && prof.lpf_dt > 0.0)
    {
        pt1FilterInit(&dterm_filter, prof.dterm_lpf_hz, prof.lpf_dt);
        use_dterm_filter = true;
    }

    reset();
}

void PID::reset()
{
    iterm = 0.0;
    last_d_error = 0.0;
}

float PID::update(float input, float setpoint)
{
	const float error_temp = input - setpoint;

    float pterm = prof.kp * error_temp;

	iterm += prof.ki * error_temp;
	iterm = constrain(iterm, -prof.max_i, prof.max_i);

	float dterm = prof.kd * (error_temp - last_d_error);
	last_d_error = error_temp;

    if (use_dterm_filter)
    {
	    dterm = pt1FilterApply(&dterm_filter, dterm);
    }

	output = pterm + iterm + dterm;
	output = constrain(output, -prof.max_out, prof.max_out);

    return get_output();
}

float PID::get_output() const
{
    return output;
}

void PID::set_gains(const float kp, const float ki, const float kd)
{
    prof.kp = kp;
    prof.ki = ki;
    prof.kd = kd;
}