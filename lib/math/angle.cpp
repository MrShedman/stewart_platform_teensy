#include "angle.h"

#include <math.h>

Angle Angle::fromDegrees(const float deg)
{
    // Normalize to [0, 360]
    float new_deg = fmodf(deg, 360.0);
    if (new_deg < 0.0)
        new_deg += 360.0;

    return Angle(new_deg * M_PI / 180.0);
}

Angle Angle::fromRadians(const float rad)
{
    return Angle(rad);
}

Angle::Angle(float rad)
{
    m_radians = rad;
}

float Angle::asDegrees() const
{
    return m_radians * 180.0 / M_PI;
}

float Angle::asRadians() const
{
    return m_radians;
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
	return Angle::fromRadians(-right.asRadians());
}

Angle operator +(Angle left, Angle right)
{
	return Angle::fromRadians(left.asRadians() + right.asRadians());
}

Angle& operator +=(Angle& left, Angle right)
{
	return left = left + right;
}

Angle operator -(Angle left, Angle right)
{
	return Angle::fromRadians(left.asRadians() - right.asRadians());
}

Angle& operator -=(Angle& left, Angle right)
{
	return left = left - right;
}

Angle operator *(Angle left, float right)
{
	return Angle::fromRadians(left.asRadians() * right);
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
	return Angle::fromRadians(left.asRadians() / right);
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
	return Angle::fromRadians(fmodf(left.asRadians(), right.asRadians()));
}

Angle& operator %=(Angle& left, Angle right)
{
	return left = left % right;
}