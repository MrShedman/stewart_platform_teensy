#pragma once

#include <Arduino.h>

class Servo
{
public:

	void begin(uint8_t pin, float min, float max);

	void setFrequency(float freq);

	void setResolution(uint8_t bits);

	void write(float usec);

private:

	float m_frequency;
	uint8_t m_analog_res_bits;
	float m_analog_scale;

	uint8_t m_pin;
	float m_min;
	float m_max;
};