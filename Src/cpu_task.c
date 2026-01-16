#include <os.h>
#include <stdio.h>
#include "oled.h"

#define  APP_TASK_STATUS_PRIO                       6
#define  APP_TASK_STATUS_STK_SIZE                   256

extern OS_TCB LedTaskTCB;
extern OS_TCB UartTaskTCB;

static OS_TCB AppTaskStatusTCB;
static CPU_STK AppTaskStatusStk[APP_TASK_STATUS_STK_SIZE];

static void showCPU() {
    u8 display_buf[32] = {0};

    sprintf(display_buf, "CPU: %d.%d%%", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100);

    OLED_Display_GB2312_string(0, 2, display_buf);
}

static void task(void *p_arg) {
    OS_ERR err;
    u8 display_buf[32] = {0};
    
    (void) p_arg;

    while (1) {
        sprintf(display_buf, "CPU: %d.%d%%", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100);

        OLED_Display_GB2312_string(0, 2, display_buf);

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_PERIODIC, &err);

        showCPU();
    }
}

void createCPUTask(void) {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & AppTaskStatusTCB,
                 (CPU_CHAR *) "App Task Status",
                 (OS_TASK_PTR) task,
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
