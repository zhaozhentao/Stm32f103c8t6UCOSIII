#include <os.h>
#include <stdio.h>
#include "prio.h"
#include "stm32f1xx_hal.h"

#define  APP_TASK_STATUS_STK_SIZE                   128

extern OS_MUTEX gTimeMutex;

static OS_TCB AppTaskStatusTCB;
static CPU_STK AppTaskStatusStk[APP_TASK_STATUS_STK_SIZE];

uint32_t gSysUnixTime = 0;

static void task(void *p_arg) {
    OS_ERR err;

    (void) p_arg;

    while (1) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_PERIODIC, &err);

        // 未初始化
        if (gSysUnixTime == 0) {
            continue;
        }

        OSMutexPend(&gTimeMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        gSysUnixTime++;   // 递增 1 秒
        OSMutexPost(&gTimeMutex, OS_OPT_POST_NONE, &err);
    }
}

void createCPUTask(void) {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & AppTaskStatusTCB,
                 (CPU_CHAR *) "CPU Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) TIMER_TASK_PRIO,
                 (CPU_STK * ) & AppTaskStatusStk[0],
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR * ) & err);
}
