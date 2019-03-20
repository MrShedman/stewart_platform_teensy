#include "vec3.h"
#include "quat.h"

Vec3 rotate(const Vec3& v, const Quat& rot)
{
	const Vec3 u(rot.x, rot.y, rot.z);
	const float s = rot.w;

	const Vec3 t = cross(u, v) * 2.0;
	return v + t * s + cross(u, t);
}