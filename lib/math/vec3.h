#pragma once

class Quat;

class Vec3
{
public:

    Vec3();

    Vec3(const float x, const float y, const float z);

    union
    {
        struct {float x,y,z;};
        float el[3];
    };
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs);

Vec3 operator-(const Vec3& lhs, const Vec3& rhs);

Vec3 operator*(const Vec3& lhs, const float rhs);

Vec3 operator/(const Vec3& lhs, const float rhs);

Vec3& operator+=(Vec3& lhs, const Vec3& rhs);

Vec3& operator-=(Vec3& lhs, const Vec3& rhs);

Vec3& operator*=(Vec3& lhs, const float rhs);

Vec3& operator/=(Vec3& lhs, const float rhs);

bool operator==(const Vec3& lhs, const Vec3& rhs);

bool operator!=(const Vec3& lhs, const Vec3& rhs);

float dot(const Vec3& lhs, const Vec3& rhs);

Vec3 cross(const Vec3& lhs, const Vec3& rhs);

float triple(const Vec3& v1, const Vec3& v2, const Vec3& v3);

float length_sq(const Vec3& v);

float length(const Vec3& v);

Vec3 normalise(const Vec3& v);

Vec3 rotate(const Vec3& v, const Quat& rot);