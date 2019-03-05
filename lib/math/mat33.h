#pragma once

#include "vec3.h"
#include "quat.h"

#include <math.h>

class Mat33
{
public:

    void setRPY(const Vec3& euler)
    {
        const float cx = cosf(euler.x);
        const float sx = sinf(euler.x);
        const float cy = cosf(euler.y);
        const float sy = sinf(euler.y); 
        const float cz = cosf(euler.z);
        const float sz = sinf(euler.z);

        const float cc = cx * cz;
        const float cs = cx * sz;
        const float sc = sx * cz;
        const float ss = sx * sz;

        m[0] = Vec3(cy * cz, sy * sc - cs, sy * cc + ss);
        m[1] = Vec3(cy * sz, sy * ss + cc, sy * cs - sc);
        m[2] = Vec3(-sy,     cy * sx,      cy * cx);
    }

    Vec3 getRPY() const
    {
        Vec3 euler;

        if (fabsf(m[2].x) >= 1.0)
        {
            euler.z = 0.0;

            // From difference of angles formula
            if (m[2].x < 0.0)  //gimbal locked down
            {
                euler.y = PI / 2.0;
                euler.x = atan2f(m[0].y, m[0].z);
            }
            else // gimbal locked up
            {
                euler.y = -PI / 2.0;
                euler.x = atan2f(-m[0].y, -m[0].z);
            }
        }
        else
        {
            euler.y = -asinf(m[2].x);
            euler.x = atan2f(m[2].y / cosf(euler.y), m[2].z / cosf(euler.y));
            euler.z = atan2f(m[1].x / cosf(euler.y), m[0].x / cosf(euler.y));
        }

        return euler;
    }

    void setQuat(const Quat& q)
    {
        const float d = dot(q, q);
        const float s = 2.0 / d;

        const float xs = q.x * s,   ys = q.y * s,   zs = q.z * s;
        const float wx = q.w * xs,  wy = q.z * ys,  wz = q.w * zs;
        const float xx = q.x * xs,  xy = q.x * ys,  xz = q.x * zs;
        const float yy = q.y * ys,  yz = q.y * zs,  zz = q.z * zs;

        m[0] = Vec3(1.0 - (yy + zz),    xy - wz,            xz + wy);
        m[1] = Vec3(xy + wz,            1.0 - (xx + zz),    yz - wx);
        m[2] = Vec3(xz - wy,            yz + wx,            1.0 - (xx + yy));
    }

    Vec3 m[3];
};

Vec3 operator*(const Mat33& m, const Vec3& v) 
{
	return Vec3(dot(m.m[0], v), dot(m.m[1], v), dot(m.m[2], v));
}