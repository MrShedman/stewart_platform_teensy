#pragma once

#include <Arduino.h>

#include "time.h"

const Time SCHEDULER_DELAY_LIMIT = microseconds(10);
const uint32_t MAX_TASK_COUNT = 20;
const uint32_t MOVING_SUM_COUNT = 32;

enum TaskPriority_e 
{
    TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
    TASK_PRIORITY_LOW = 1,
    TASK_PRIORITY_MEDIUM = 3,
    TASK_PRIORITY_MEDIUM_HIGH = 4,
    TASK_PRIORITY_HIGH = 5,
    TASK_PRIORITY_REALTIME = 6,
    TASK_PRIORITY_MAX = 255
};

struct CheckFuncInfo_t
{
    Time     maxExecutionTime;
    Time     totalExecutionTime;
    Time     averageExecutionTime;
};

struct TaskInfo_t
{
    const char * taskName;
    uint8_t      id;
    bool         isEnabled;
    uint8_t      staticPriority;
    Time         desiredPeriod;
    Time         latestDeltaTime;
    Time         maxExecutionTime;
    Time         totalExecutionTime;
    Time         averageExecutionTime;
};

struct Task_t 
{
    typedef bool (checkFunctionPtr)(const Time& currentTimeUs, const Time& currentDeltaTimeUs);
    typedef void (taskFunctionPtr)(const Time& currentTimeUs);

    Task_t(const char* taskName, checkFunctionPtr* checkFunc_ptr, taskFunctionPtr* taskFunc_ptr, Time desired_period, const uint8_t priority)
    :
    id(255),
    taskName(taskName),
    checkFunc(checkFunc_ptr),
    taskFunc(taskFunc_ptr),
    desiredPeriod(desired_period),
    staticPriority(priority)
    {

    }
  
    // Configuration
    uint8_t id;
    const char* taskName;
	checkFunctionPtr* checkFunc;
	taskFunctionPtr* taskFunc;
    Time desiredPeriod;      // target period of execution
    const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

    // Scheduling
    uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
    uint16_t taskAgeCycles;
    Time taskLatestDeltaTime;
    Time lastExecutedAt;        // last time of invocation
    Time lastSignaledAt;        // time of invocation event for event-driven tasks

    // Statistics
    Time movingSumExecutionTime;  // moving sum over 32 samples
    Time maxExecutionTime;
    Time totalExecutionTime;    // total time consumed by task since boot
};

void printTaskInfo(Task_t* task);
void getCheckFuncInfo(CheckFuncInfo_t *checkFuncInfo);
void getTaskInfo(Task_t* task, TaskInfo_t *taskInfo);
void rescheduleTask(Task_t* task, Time newPeriod);
bool addTask(Task_t* task);
void setTaskEnabled(Task_t* task, bool newEnabledState);
Time getTaskDeltaTime(Task_t* task);
void schedulerSetCalulateTaskStatistics(bool calculateTaskStatistics);
void schedulerResetTaskStatistics(Task_t* task);
uint16_t getAverageSystemLoadPercent();

void schedulerInit();
void scheduler();
