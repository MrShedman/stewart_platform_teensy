#pragma once

#undef radians
#undef degrees

class Angle
{
public:

    static Angle fromDegrees(const float deg);
    static Angle fromRadians(const float rad);

    float asDegrees() const;
    float asRadians() const;

private:

	Angle(const float rad);

	float m_radians;
};

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