#include <Arduino.h>

#include "platform.h"
#include "pattern_manager.h"
#include "controller.h"
#include "rate.h"
#include "connectivity.h"
#include "scheduler.h"

Platform platform;
PatternManager pattern_manager;

Controller controller(platform, pattern_manager);

Rate rate(100.0);

void update_control(uint32_t currentTimeUs)
{
    controller.update();
}

Task_t control_task("control_task", nullptr, update_control, TASK_PERIOD_HZ(500), TASK_PRIORITY_REALTIME);

Task_t blynk_run_task("blynk_run_task", nullptr, update_blynk, TASK_PERIOD_HZ(200), TASK_PRIORITY_HIGH);
Task_t blynk_sync_task("blynk_sync_task", nullptr, sync_params, TASK_PERIOD_HZ(20), TASK_PRIORITY_MEDIUM_HIGH);

void init_scheduler()
{
    schedulerInit();
    addTask(&control_task);
    addTask(&blynk_run_task);
    addTask(&blynk_sync_task);

    setTaskEnabled(&control_task, true);
    setTaskEnabled(&blynk_run_task, true);
    setTaskEnabled(&blynk_sync_task, true);
}

void setup() 
{
    init_blynk();

    platform.init_servos();

    init_scheduler();
}

void loop()
{
    scheduler();
}