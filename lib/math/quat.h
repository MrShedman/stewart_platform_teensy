#pragma once

class Quat
{
public:

    Quat()
    :
    x(0.0),
    y(0.0),
    z(0.0),
    w(1.0)
    {}

    Quat(const float x, const float y, const float z, const float w = 1.0)
    :
    x(x),
    y(y),
    z(z),
    w(w)
    {}

    float x;
    float y;
    float z;
    float w;
};

float dot(const Quat& lhs, const Quat& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Quat operator*(const Quat& lhs, const Quat& rhs)
{
    const float nw = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
    const float nx = (lhs.x * rhs.w) + (lhs.w * rhs.x) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
    const float ny = (lhs.y * rhs.w) + (lhs.w * rhs.y) + (lhs.z * rhs.x) - (lhs.x * rhs.z);
    const float nz = (lhs.z * rhs.w) + (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x);

    return Quat(nx, ny, nz, nw);
}