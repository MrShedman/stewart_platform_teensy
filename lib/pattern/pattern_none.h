#pragma once

#include "pattern_base.h"

class PatternNone : public PatternBase
{
public:

    PatternNone()
    :
    PatternBase("None", 0.0)
    {}

    const Vec3& update() override
    {
        return m_point;
    }
};