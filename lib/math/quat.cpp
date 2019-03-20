#include "quat.h"

#include "vec3.h"
#include "angle.h"

#include <math.h>

Quat::Quat()
:
x(0.0),
y(0.0),
z(0.0),
w(1.0)
{}

Quat::Quat(const float x, const float y, const float z, const float w)
:
x(x),
y(y),
z(z),
w(w)
{}

Quat::Quat(const Vec3& axis, const Angle& angle)
{
    const float sin_half = sinf(angle.asRadians() * 0.5);
    const float cos_half = cosf(angle.asRadians() * 0.5);

    x = axis.x * sin_half;
    y = axis.y * sin_half;
    z = axis.z * sin_half;
    w = cos_half;
}

float dot(const Quat& lhs, const Quat& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

float length_sq(const Quat& q)
{
    return dot(q, q);
}

float length(const Quat& q)
{
    return sqrtf(length_sq(q));
}

Quat normalise(const Quat& q)
{
    const float l = length(q);
    return Quat(q.x / l, q.y / l, q.z / l, q.w / l);
}

Quat operator*(const Quat& lhs, const Quat& rhs)
{
    const float nw = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
    const float nx = (lhs.x * rhs.w) + (lhs.w * rhs.x) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
    const float ny = (lhs.y * rhs.w) + (lhs.w * rhs.y) + (lhs.z * rhs.x) - (lhs.x * rhs.z);
    const float nz = (lhs.z * rhs.w) + (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x);

    return Quat(nx, ny, nz, nw);
}