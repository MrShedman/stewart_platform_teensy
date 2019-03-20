#pragma once

#include <math.h>

class Vec3
{
public:

    Vec3()
    :
    x(0.0),
    y(0.0),
    z(0.0)
    {}

    Vec3(const float x, const float y, const float z)
    :
    x(x),
    y(y),
    z(z)
    {}

    union
    {
        struct {float x,y,z;};
        float el[3];
    };
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vec3 operator*(const Vec3& lhs, const float rhs)
{
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vec3 operator/(const Vec3& lhs, const float rhs)
{
    return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

Vec3& operator+=(Vec3& lhs, const Vec3& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;

    return lhs;
}

Vec3& operator-=(Vec3& lhs, const Vec3& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;

    return lhs;
}

Vec3& operator*=(Vec3& lhs, const float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;

    return lhs;
}

Vec3& operator/=(Vec3& lhs, const float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;

    return lhs;
}

bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vec3& lhs, const Vec3& rhs)
{
    return !operator==(lhs, rhs);
}

float dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    const float nx = lhs.y * rhs.z - lhs.z * rhs.y;
    const float ny = lhs.z * rhs.x - lhs.x * rhs.z;
    const float nz = lhs.x * rhs.y - lhs.y * rhs.x;

    return Vec3(nx, ny, nz);
}

float triple(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
    return dot(v1, cross(v2, v3));
}

float length_sq(const Vec3& v)
{
    return dot(v, v);
}

float length(const Vec3& v)
{
    return sqrtf(length_sq(v));
}

Vec3 normalise(const Vec3& v)
{
    return v / length(v);
}

class Quat;

Vec3 rotate(const Vec3& v, const Quat& rot);