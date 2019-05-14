#pragma once

#include "vec3.h"

#include <limits>
#include <vector>

class AABB
{
public:

	AABB()
    :
    m_min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
    m_max(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())
	{}

    AABB(const std::vector<Vec3>& point_array)
    {
        include(point_array);
    }

	void include(const Vec3& point)
	{
		m_min = ::min(m_min, point);
		m_max = ::max(m_max, point);
	}

    void include(const std::vector<Vec3>& point_array)
    {
        for (std::size_t i = 0; i < point_array.size(); ++i)
        {
            include(point_array[i]);
        }
    }

    bool contains(const Vec3& point)
    {
        return (point.x > m_min.x && point.x < m_max.x &&
            point.y > m_min.y && point.y < m_max.y &&
            point.z > m_min.z && point.z < m_max.z);
    }

    const Vec3& min() const
    {
        return m_min;
    }

    const Vec3& max() const
    {
        return m_max;
    }

	float getWidth() const
	{
		return m_max.x - m_min.x;
	}

	float getHeight() const
	{
		return m_max.y - m_min.y;
	}

	float getDepth() const
	{
		return m_max.z - m_min.z;
	}

	float getVolume() const
	{
		return getWidth() * getHeight() * getDepth();
	}

private:

	Vec3 m_min;
	Vec3 m_max;
};