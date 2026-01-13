#include <os.h>
#include "stm32f1xx_hal.h"
#include <stdio.h>

#define  APP_TASK_STATUS_PRIO                       2
#define  APP_TASK_STATUS_STK_SIZE                   256

extern OS_TCB LedTaskTCB;

static OS_TCB AppTaskStatusTCB;
static CPU_STK AppTaskStatusStk[APP_TASK_STATUS_STK_SIZE];

void AppTaskStatus(void *p_arg) {
    OS_ERR err;

    CPU_SR_ALLOC();

    (void) p_arg;

    while (DEF_TRUE) {
        OS_CRITICAL_ENTER();                              //进入临界段，避免串口打印被打断
        printf("------------------------------------------------------------\n");
        printf("CPU利用率：%d.%d%%\r\n", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100);
        printf("CPU最大利用率：%d.%d%%\r\n", OSStatTaskCPUUsageMax / 100, OSStatTaskCPUUsageMax % 100);

        printf("LED1任务的CPU利用率：%d.%d%%\r\n", LedTaskTCB.CPUUsageMax / 100, LedTaskTCB.CPUUsageMax % 100);
        printf("统计任务的CPU利用率：%d.%d%%\r\n", AppTaskStatusTCB.CPUUsageMax / 100,
               AppTaskStatusTCB.CPUUsageMax % 100);

        printf("LED1任务的已用和空闲堆栈大小分别为：%d,%d\r\n", LedTaskTCB.StkUsed, LedTaskTCB.StkFree);
        printf("统计任务的已用和空闲堆栈大小分别为：%d,%d\r\n", AppTaskStatusTCB.StkUsed, AppTaskStatusTCB.StkFree);

        printf("------------------------------------------------------------\n");
        OS_CRITICAL_EXIT();                               //退出临界段

        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_PERIODIC, &err);
    }
}

void createCPUTask(void) {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & AppTaskStatusTCB,
                 (CPU_CHAR *) "App Task Status",
                 (OS_TASK_PTR) AppTaskStatus,
                 (void *) 0,
                 (OS_PRIO) APP_TASK_STATUS_PRIO,
                 (CPU_STK * ) & AppTaskStatusStk[0],
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR * ) & err);
}
