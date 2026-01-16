#include <os.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "oled.h"

#define LED_TASK_STK_SIZE 256

static OS_TCB TaskTCB;
static CPU_STK TaskStk[LED_TASK_STK_SIZE];

extern uint32_t gSysUnixTime;

static void showTime() {
    u8 buffer[32] = {0};

    time_t unixTime = gSysUnixTime;

    struct tm *tmInfo = localtime(&unixTime);

    strftime(buffer, sizeof(buffer), "%m/%d %H:%M:%S", tmInfo);

    OLED_Display_GB2312_string(0, 0, buffer);
}

static void showCPU() {
    u8 display_buf[32] = {0};

    sprintf(display_buf, "CPU: %d.%d%%", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100);

    OLED_Display_GB2312_string(0, 2, display_buf);
}

static void task() {
    OS_ERR err;

    while (1) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        showTime();

        showCPU();
    }
}

void createTask3() {
    OSTaskCreate((OS_TCB * ) & TaskTCB,
                 (CPU_CHAR *) "LED Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) 9,
                 (CPU_STK * ) & TaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
