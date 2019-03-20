#pragma once

#include <Arduino.h>

#undef radians
#undef degrees

class Angle
{
public:

	Angle()
     :
    m_radians(0.0)
    {}

	Angle(float angle_deg)
    {
        // Normalize to [0, 360]
        angle_deg = fmodf(angle_deg, 360.0);
        if (angle_deg < 0.0)
            angle_deg += 360.0;

        m_radians = angle_deg * M_PI / 180.0;
    }

    float asDegrees() const
    {
        return m_radians * 180.0 / M_PI;
    }

    float asRadians() const
    {
        return m_radians;
    }

private:

	float m_radians;
};

Angle degrees(float amount)
{
	return Angle(amount);
}

Angle radians(float amount)
{
	return Angle(amount * 180.0 / M_PI);
}

bool operator ==(Angle left, Angle right)
{
	return left.asRadians() == right.asRadians();
}

bool operator !=(Angle left, Angle right)
{
	return left.asRadians() != right.asRadians();
}

bool operator <(Angle left, Angle right)
{
	return left.asRadians() < right.asRadians();
}

bool operator >(Angle left, Angle right)
{
	return left.asRadians() > right.asRadians();
}

bool operator <=(Angle left, Angle right)
{
	return left.asRadians() <= right.asRadians();
}

bool operator >=(Angle left, Angle right)
{
	return left.asRadians() >= right.asRadians();
}

Angle operator -(Angle right)
{
	return radians(-right.asRadians());
}

Angle operator +(Angle left, Angle right)
{
	return radians(left.asRadians() + right.asRadians());
}

Angle& operator +=(Angle& left, Angle right)
{
	return left = left + right;
}

Angle operator -(Angle left, Angle right)
{
	return radians(left.asRadians() - right.asRadians());
}

Angle& operator -=(Angle& left, Angle right)
{
	return left = left - right;
}

Angle operator *(Angle left, float right)
{
	return radians(left.asRadians() * right);
}

Angle operator *(float left, Angle right)
{
	return right * left;
}

Angle& operator *=(Angle& left, float right)
{
	return left = left * right;
}

Angle operator /(Angle left, float right)
{
	return radians(left.asRadians() / right);
}

Angle& operator /=(Angle& left, float right)
{
	return left = left / right;
}

float operator /(Angle left, Angle right)
{
	return left.asRadians() / right.asRadians();
}

Angle operator %(Angle left, Angle right)
{
	return radians(fmodf(left.asRadians(), right.asRadians()));
}

Angle& operator %=(Angle& left, Angle right)
{
	return left = left % right;
}