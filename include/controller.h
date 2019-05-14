#pragma once

#include "platform.h"
#include "pid.h"
#include "touch.h"
#include "pattern_manager.h"
#include "mat33.h"

class Controller
{
public:

    Controller(Platform& platform, PatternManager& pattern_manager)
    :
    m_platform(platform),
    m_pattern_manager(pattern_manager)
    {
        pidProfile_t p;

        p.kp = 1.0;
        p.ki = 0.1;
        p.kd = 0.0;
        p.max_i = 5.0;
        p.max_out = 10.0;
        p.dterm_lpf_hz = 0.0f;
        p.lpf_dt = 0.0f;

        m_xpid.init(p);
        m_ypid.init(p);
    }

    void update()
    {
        const Touch& t = get_touch_list().back();

        const Vec3 setpoint = m_pattern_manager.update();

        float pid_x = 0.0;
        float pid_y = 0.0;

        if (t.contact)
        {
            pid_x = m_xpid.update(t.pos.x, setpoint.x);
            pid_y = m_ypid.update(t.pos.y, setpoint.y);
        }

        Mat33 rot;
        rot.setRPY(Vec3(pid_x, pid_y, 0.0));

        m_platform.applyTranslationAndRotation(Vec3(), rot);
    }

private:

    Platform& m_platform;
    PID m_xpid;
    PID m_ypid;
    PatternManager& m_pattern_manager;
};