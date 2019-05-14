#pragma once

#include "pattern_base.h"
#include "line.h"

#include <array>

template <std::size_t SIZE>
class PatternPolyLine : public PatternBase
{
public:

    PatternPolyLine(const std::array<Vec3, SIZE>& point_array, const float speed)
    :
    PatternBase(speed),
    m_line_index(0)
    {
        for (std::size_t i = 0; i < SIZE - 1; ++i)
        {
            m_lines[i] = Line(point_array[i], point_array[i + 1]);
        }
    }

    const Vec3& update() override
    {
        const float d = get_delta();

        m_point = m_lines[m_line_index].update(d);
        if (m_lines[m_line_index].complete())
        {
            m_lines[m_line_index].reset();
            m_line_index++;
            m_line_index %= SIZE;
        }
        return m_point;
    }

private:

    std::size_t m_line_index;
    std::array<Line, SIZE> m_lines;
};