#include <Arduino.h>

#include "platform.h"
#include "pattern_manager.h"
#include "controller.h"
#include "touch.h"
#include "connectivity.h"
#include "scheduler.h"

Task_t control_task("control_task", nullptr, update_control, TASK_PERIOD_HZ(500), TASK_PRIORITY_REALTIME);
Task_t touch_task("touch_task", nullptr, update_touch, TASK_PERIOD_HZ(TOUCH_FRAME_RATE), TASK_PRIORITY_REALTIME);

Task_t blynk_run_task("blynk_run_task", nullptr, update_blynk, TASK_PERIOD_HZ(200), TASK_PRIORITY_HIGH);
Task_t blynk_sync_task("blynk_sync_task", nullptr, sync_params, TASK_PERIOD_HZ(20), TASK_PRIORITY_MEDIUM_HIGH);

void init_scheduler()
{
    schedulerInit();
    addTask(&control_task);
    addTask(&touch_task);
    addTask(&blynk_run_task);
    addTask(&blynk_sync_task);

    setTaskEnabled(&control_task, true);
    setTaskEnabled(&touch_task, true);
    setTaskEnabled(&blynk_run_task, true);
    setTaskEnabled(&blynk_sync_task, true);
}

void setup() 
{
    init_blynk();

    init_platform();

    init_pattern_manager();

    init_scheduler();
}

void loop()
{
    scheduler();
}