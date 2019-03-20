#pragma once

#undef radians
#undef degrees

class Angle
{
public:

	Angle();

	Angle(float angle_deg);

    float asDegrees() const;

    float asRadians() const;

private:

	float m_radians;
};

Angle degrees(float amount);

Angle radians(float amount);

bool operator ==(Angle left, Angle right);

bool operator !=(Angle left, Angle right);

bool operator <(Angle left, Angle right);

bool operator >(Angle left, Angle right);

bool operator <=(Angle left, Angle right);

bool operator >=(Angle left, Angle right);

Angle operator -(Angle right);

Angle operator +(Angle left, Angle right);

Angle& operator +=(Angle& left, Angle right);

Angle operator -(Angle left, Angle right);

Angle& operator -=(Angle& left, Angle right);

Angle operator *(Angle left, float right);

Angle operator *(float left, Angle right);

Angle& operator *=(Angle& left, float right);

Angle operator /(Angle left, float right);

Angle& operator /=(Angle& left, float right);

float operator /(Angle left, Angle right);

Angle operator %(Angle left, Angle right);

Angle& operator %=(Angle& left, Angle right);