#pragma once

#include "Arduino.h"
#include "filter.h"

struct pidProfile_t
{
	float kp;
	float ki;
	float kd;

	float max_I;
	float max_Out;

	float dterm_lpf_hz;
	float lpf_dT;
};

class PID
{
public:

	void init(const pidProfile_t& profile);

	float update(float input, float setpoint);

	void reset();

	float get_output() const;

private:

	float p_gain;
	float i_gain;
	float d_gain;

	float max_output;
	float max_i;

	float i_mem;
	float output;
	float last_d_error;

	pt1Filter_t dterm_filter;
};