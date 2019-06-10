#include "controller.h"

#include "platform.h"
#include "pid.h"
#include "touch.h"
#include "pattern_manager.h"
#include "mat33.h"
#include "logger.h"
#include "clock.h"

Mat33 m_rot;
Vec3 m_pid_output;
Vec3 m_target_pos;
Clock m_user_control_timer;
const Time m_user_control_timeout = seconds(1.0);
PID m_xpid;
PID m_ypid;
bool m_error = false;

void init_control()
{
    pidProfile_t p;

    p.kp = 1.0;
    p.ki = 0.1;
    p.kd = 0.0;
    p.max_i = 5.0;
    p.max_out = 10.0;
    p.dterm_lpf_hz = 0.0f;
    p.lpf_dt = seconds(0.0);

    m_xpid.init(p);
    m_ypid.init(p);
}

void update_control(const Time& currentTime)
{
    const Touch& t = get_touch_list().back();

    if (t.contact && !m_error)
    {
        if (m_user_control_timer.get_elapsed_time() > m_user_control_timeout)
        {
            m_target_pos = update_pattern_manager();
        }

        LOG_INFO("%.2f, %.2f", m_target_pos.x, m_target_pos.y);

        m_pid_output.x = m_xpid.update(t.pos.x, m_target_pos.x);
        m_pid_output.y = m_ypid.update(t.pos.y, m_target_pos.y);
    }
    else
    {
        m_xpid.reset();
        m_ypid.reset();

        m_pid_output.x = 0.0;
        m_pid_output.y = 0.0;
    }

    m_rot.setRPY(m_pid_output);

    if (!transform_platform(Vec3(), m_rot))
    {
        m_error = true;
        m_target_pos = Vec3();
        m_rot.setRPY(Vec3());
        home_platform();
    }
}

void reset_errors()
{
    m_error = false;
}

bool has_errors()
{
    return m_error;
}

void set_target_pos(const Vec3& pos)
{
    m_target_pos = pos;
    m_user_control_timer.restart();
}