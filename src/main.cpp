#include <Arduino.h>

#include "platform.h"
#include "pattern_manager.h"
#include "controller.h"
#include "touch.h"
#include "connectivity.h"
#include "scheduler.h"
#include "task.h"
#include "logger_serial.h"

LoggerSerial logger;

Task control_task(    "control_task",     nullptr, update_control,    hertz(200.0),               TASK_PRIORITY_REALTIME);
Task touch_task(      "touch_task",       nullptr, update_touch,      hertz(TOUCH_FRAME_RATE),    TASK_PRIORITY_REALTIME);
Task blynk_run_task(  "blynk_run_task",   nullptr, update_blynk,      hertz(200.0),               TASK_PRIORITY_HIGH);
Task blynk_sync_task( "blynk_sync_task",  nullptr, sync_params,       hertz(20.0),                TASK_PRIORITY_MEDIUM_HIGH);

void init_scheduler()
{
    schedulerInit();
    addTask(&control_task);
    addTask(&touch_task);
    addTask(&blynk_run_task);
    addTask(&blynk_sync_task);

    setTaskEnabled(&control_task, true);
    //setTaskEnabled(&touch_task, true);
    //setTaskEnabled(&blynk_run_task, true);
    //setTaskEnabled(&blynk_sync_task, true);
}

void setup() 
{
    while(!Serial);

    LOG_INFO("Starting Stewart Platform!");

    ///init_blynk();

    //init_platform();#

    init_pattern_manager();

    init_control();

    init_scheduler();

    LOG_INFO("Scheduler started!");
}

void loop()
{
    scheduler();
}