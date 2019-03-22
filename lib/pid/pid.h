#pragma once

#include "filter.h"

struct pidProfile_t
{
	float kp;
	float ki;
	float kd;

	float max_i;
	float max_out;

	float dterm_lpf_hz;
	float lpf_dt;
};

class PID
{
public:

	void init(const pidProfile_t& profile);

	void reset();

	float update(float input, float setpoint);

	float get_output() const;

    void set_gains(const float kp, const float ki, const float kd);

private:

	pidProfile_t prof;

	float iterm;
	float output;
	float last_d_error;

    bool use_dterm_filter;
	pt1Filter_t dterm_filter;
};