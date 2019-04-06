#include "servo.h"

#include "logger.h"

void Servo::begin(uint8_t pin, float min, float max)
{
    if (!digitalPinHasPWM(pin))
    {
        LOG_FATAL("Pin %d does not support PWM!", (uint32_t)pin);
        return;
    }

    m_pin = pin;
    m_min = min;
    m_max = max;

    setFrequency(50.0);
    setResolution(12);

    pinMode(m_pin, OUTPUT);
    write(m_min);
}

void Servo::setFrequency(float freq)
{
    m_frequency = freq;
    analogWriteFrequency(m_pin, m_frequency);
}

void Servo::setResolution(uint8_t bits)
{
    m_analog_res_bits = bits;
    m_analog_scale = powf(2.0, (float)m_analog_res_bits);
    analogWriteResolution(m_analog_res_bits);
}

void Servo::write(float usec)
{
    usec = constrain(usec, m_min, m_max);
    const uint32_t duty = (uint32_t)(usec / (1e6 / m_frequency) * m_analog_scale);
    analogWrite(m_pin, duty);
}
