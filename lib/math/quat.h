#pragma once

class Vec3;
class Angle;

class Quat
{
public:

    Quat();

    Quat(const float x, const float y, const float z, const float w = 1.0);

    Quat(const Vec3& axis, const Angle& angle);

    float x;
    float y;
    float z;
    float w;
};

float dot(const Quat& lhs, const Quat& rhs);

float length_sq(const Quat& q);

float length(const Quat& q);

Quat normalise(const Quat& q);

Quat operator*(const Quat& lhs, const Quat& rhs);