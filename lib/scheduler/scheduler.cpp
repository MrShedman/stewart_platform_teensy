#include "scheduler.h"

#include "logger.h"

namespace
{
    Task_t *currentTask = nullptr;

    uint32_t totalWaitingTasks;
    uint32_t totalWaitingTasksSamples;

    bool calculateTaskStatistics;
    uint16_t averageSystemLoadPercent = 0;

    uint32_t taskQueuePos = 0;
    uint32_t taskQueueSize = 0;

    Time checkFuncMaxExecutionTime;
    Time checkFuncTotalExecutionTime;
    Time checkFuncMovingSumExecutionTime;

    Task_t* Tasks[MAX_TASK_COUNT];
    uint8_t task_count = 0;

    Task_t* taskQueueArray[MAX_TASK_COUNT + 1]; // extra item for nullptr pointer at end of queue
}

void taskSystem(const Time& currentTime)
{
    // Calculate system load
    if (totalWaitingTasksSamples > 0)
	{
        averageSystemLoadPercent = 100 * totalWaitingTasks / totalWaitingTasksSamples;
        totalWaitingTasksSamples = 0;
        totalWaitingTasks = 0;
    }
}

uint16_t getAverageSystemLoadPercent()
{
    return averageSystemLoadPercent;
}

Task_t task_sys("SYSTEM", nullptr, taskSystem, hertz(10.0), TASK_PRIORITY_MEDIUM_HIGH);

void queueClear()
{
    memset(taskQueueArray, 0, sizeof(taskQueueArray));
    taskQueuePos = 0;
    taskQueueSize = 0;
}

bool queueContains(Task_t *task)
{
    for (uint32_t ii = 0; ii < taskQueueSize; ++ii)
	{
        if (taskQueueArray[ii] == task)
		{
            return true;
        }
    }
    return false;
}

bool queueAdd(Task_t *task)
{
    if ((taskQueueSize >= task_count) || queueContains(task))
	{
        return false;
    }
    for (uint32_t ii = 0; ii <= taskQueueSize; ++ii)
	{
        if (taskQueueArray[ii] == nullptr || taskQueueArray[ii]->staticPriority < task->staticPriority) 
		{
            memmove(&taskQueueArray[ii+1], &taskQueueArray[ii], sizeof(task) * (taskQueueSize - ii));
            taskQueueArray[ii] = task;
            ++taskQueueSize;
            return true;
        }
    }
    return false;
}

bool queueRemove(Task_t *task)
{
    for (uint32_t ii = 0; ii < taskQueueSize; ++ii) 
	{
        if (taskQueueArray[ii] == task)
		{
            memmove(&taskQueueArray[ii], &taskQueueArray[ii+1], sizeof(task) * (taskQueueSize - ii));
            --taskQueueSize;
            return true;
        }
    }
    return false;
}

Task_t *queueFirst()
{
    taskQueuePos = 0;
    return taskQueueArray[0]; // guaranteed to be nullptr if queue is empty
}

Task_t *queueNext()
{
    return taskQueueArray[++taskQueuePos]; // guaranteed to be nullptr at end of queue
}

void printTaskInfo(Task_t* task)
{
    TaskInfo_t info;
    getTaskInfo(task, &info);

    LOG_INFO("%s\n", info.taskName);
    LOG_INFO("ID: %d\n", info.id);
    LOG_INFO("Enabled: %s\n", info.isEnabled ? "true" : "false");
    LOG_INFO("Static priority: %d\n", info.staticPriority);
    LOG_INFO("Desired period: %dus\n", info.desiredPeriod.asMicroseconds());
    LOG_INFO("Latest Delta Time: %dus\n", info.latestDeltaTime.asMicroseconds());
    LOG_INFO("Max Execution Time: %dus\n", info.maxExecutionTime.asMicroseconds());
    LOG_INFO("Total Execution Time: %dus\n", info.totalExecutionTime.asMicroseconds());
    LOG_INFO("Avg Execution Time: %dus\n", info.averageExecutionTime.asMicroseconds());
}

void getCheckFuncInfo(CheckFuncInfo_t *checkFuncInfo)
{
    checkFuncInfo->maxExecutionTime = checkFuncMaxExecutionTime;
    checkFuncInfo->totalExecutionTime = checkFuncTotalExecutionTime;
    checkFuncInfo->averageExecutionTime = checkFuncMovingSumExecutionTime / (int64_t)MOVING_SUM_COUNT;
}

void getTaskInfo(Task_t* task, TaskInfo_t * taskInfo)
{
    taskInfo->taskName = Tasks[task->id]->taskName;
    taskInfo->id = Tasks[task->id]->id;
    taskInfo->isEnabled = queueContains(Tasks[task->id]);
    taskInfo->desiredPeriod = Tasks[task->id]->desiredPeriod;
    taskInfo->staticPriority = Tasks[task->id]->staticPriority;
    taskInfo->maxExecutionTime = Tasks[task->id]->maxExecutionTime;
    taskInfo->totalExecutionTime = Tasks[task->id]->totalExecutionTime;
    taskInfo->averageExecutionTime = Tasks[task->id]->movingSumExecutionTime / (int64_t)MOVING_SUM_COUNT;
    taskInfo->latestDeltaTime = Tasks[task->id]->taskLatestDeltaTime;
}

void rescheduleTask(Task_t* task, Time newPeriod)
{
    if (task->id < task_count) 
	{
		Task_t *t = Tasks[task->id];
        t->desiredPeriod = max(SCHEDULER_DELAY_LIMIT, newPeriod);  // Limit delay to 100us (10 kHz) to prevent scheduler clogging
    }
}

bool addTask(Task_t* task)
{
    if (task)
    {
        for (uint8_t i = 0; i < MAX_TASK_COUNT; ++i)
        {
            if (Tasks[i] == task)
            {
                // error task already in scheduler
                return false;
            }

            if (Tasks[i] == nullptr)
            {
                task->id = i;
                task_count++;
                Tasks[i] = task;
                return true;
            }
        }
    }

    return false;
}

void setTaskEnabled(Task_t* task, bool enabled)
{
    if (task->id < task_count) 
	{
        Task_t *t = Tasks[task->id];
        if (enabled && t->taskFunc)
		{
            queueAdd(t);
        } 
		else 
		{
            queueRemove(t);
        }
    }
}

Time getTaskDeltaTime(Task_t* task)
{
	if (task->id < task_count)
	{
        return Tasks[task->id]->taskLatestDeltaTime;
    }
	else 
	{
        return Time();
    }
}

void schedulerSetCalulateTaskStatistics(bool calculateTaskStatisticsToUse)
{
    calculateTaskStatistics = calculateTaskStatisticsToUse;
}

void schedulerResetTaskStatistics(Task_t* task)
{
    if (task->id < task_count) 
	{
        Tasks[task->id]->movingSumExecutionTime = Time();
        Tasks[task->id]->totalExecutionTime = Time();
        Tasks[task->id]->maxExecutionTime = Time();
    }
}

void schedulerInit(void)
{
	addTask(&task_sys);
    
    calculateTaskStatistics = true;

    queueClear();
    setTaskEnabled(&task_sys, true);
}

void scheduler()
{
    // Cache currentTime
    const Time currentTime = Time::now();

    // Check for realtime tasks
    bool outsideRealtimeGuardInterval = true;
    for (const Task_t *task = queueFirst(); task != nullptr && task->staticPriority >= TASK_PRIORITY_REALTIME; task = queueNext()) 
	{
        const Time nextExecuteAt = task->lastExecutedAt + task->desiredPeriod;
        if (currentTime - nextExecuteAt >= seconds(0.0)) 
		{
            outsideRealtimeGuardInterval = false;
            break;
        }
    }

    // The task to be invoked
	Task_t *selectedTask = nullptr;
    uint16_t selectedTaskDynamicPriority = 0;

    // Update task dynamic priorities
    uint16_t waitingTasks = 0;
    for (Task_t *task = queueFirst(); task != nullptr; task = queueNext()) 
	{
        // Task has checkFunc - event driven
        if (task->checkFunc)
		{
            const Time currentTimeBeforeCheckFuncCall = currentTime;
            // Increase priority for event driven tasks
            if (task->dynamicPriority > 0) 
			{
                task->taskAgeCycles = 1 + ((currentTime - task->lastSignaledAt) / task->desiredPeriod);
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            } 
			else if (task->checkFunc(currentTimeBeforeCheckFuncCall, currentTimeBeforeCheckFuncCall - task->lastExecutedAt)) 
			{
                if (calculateTaskStatistics) 
				{
                    const Time checkFuncExecutionTime = Time::now() - currentTimeBeforeCheckFuncCall;
                    checkFuncMovingSumExecutionTime += checkFuncExecutionTime - checkFuncMovingSumExecutionTime / (int64_t)MOVING_SUM_COUNT;
                    checkFuncTotalExecutionTime += checkFuncExecutionTime;   // time consumed by scheduler + task
                    checkFuncMaxExecutionTime = max(checkFuncMaxExecutionTime, checkFuncExecutionTime);
                }

                task->lastSignaledAt = currentTimeBeforeCheckFuncCall;
                task->taskAgeCycles = 1;
                task->dynamicPriority = 1 + task->staticPriority;
                waitingTasks++;
            } 
			else 
			{
                task->taskAgeCycles = 0;
            }
        } 
		else 
		{
            // Task is time-driven, dynamicPriority is last execution age (measured in desiredPeriods)
            // Task age is calculated from last execution
            task->taskAgeCycles = ((currentTime - task->lastExecutedAt) / task->desiredPeriod);
            if (task->taskAgeCycles > 0)
			{
                task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
                waitingTasks++;
            }
        }

        if (task->dynamicPriority > selectedTaskDynamicPriority) 
		{
            const bool taskCanBeChosenForScheduling =
                (outsideRealtimeGuardInterval) ||
                (task->taskAgeCycles > 1) ||
                (task->staticPriority == TASK_PRIORITY_REALTIME);
            
			if (taskCanBeChosenForScheduling)
			{
                selectedTaskDynamicPriority = task->dynamicPriority;
                selectedTask = task;
            }
        }
    }

    totalWaitingTasksSamples++;
    totalWaitingTasks += waitingTasks;

    currentTask = selectedTask;

    if (selectedTask) 
	{
        // Found a task that should be run
        selectedTask->taskLatestDeltaTime = currentTime - selectedTask->lastExecutedAt;
        selectedTask->lastExecutedAt = currentTime;
        selectedTask->dynamicPriority = 0;

        // Execute task
        if (calculateTaskStatistics) 
		{
            const Time currentTimeBeforeTaskCall = Time::now();
            selectedTask->taskFunc(currentTimeBeforeTaskCall);
            const Time taskExecutionTime = Time::now() - currentTimeBeforeTaskCall;
            selectedTask->movingSumExecutionTime += taskExecutionTime - selectedTask->movingSumExecutionTime / (int64_t)MOVING_SUM_COUNT;
            selectedTask->totalExecutionTime += taskExecutionTime;   // time consumed by scheduler + task
            selectedTask->maxExecutionTime = max(selectedTask->maxExecutionTime, taskExecutionTime);
        } 
		else 
		{
            selectedTask->taskFunc(currentTime);
        }
    }
}