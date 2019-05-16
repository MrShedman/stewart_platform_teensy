#include "controller.h"

#include "platform.h"
#include "pid.h"
#include "touch.h"
#include "pattern_manager.h"
#include "mat33.h"

Mat33 m_rot;
Vec3 m_pid_output;
PID m_xpid;
PID m_ypid;

void init_control()
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

void update_control(const Time& currentTime)
{
    const Touch& t = get_touch_list().back();

    if (t.contact)
    {
        const Vec3& setpoint = update_pattern_manager();

        m_pid_output.x = m_xpid.update(t.pos.x, setpoint.x);
        m_pid_output.y = m_ypid.update(t.pos.y, setpoint.y);
    }
    else
    {
        m_pid_output.x = 0.0;
        m_pid_output.y = 0.0;
    }

    m_rot.setRPY(m_pid_output);

    transform_platform(Vec3(), m_rot);
}