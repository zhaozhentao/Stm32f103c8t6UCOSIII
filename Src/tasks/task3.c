#include <os.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "oled.h"

#define LED_TASK_STK_SIZE 256

static OS_TCB TaskTCB;
static CPU_STK TaskStk[LED_TASK_STK_SIZE];

extern OS_Q TempMsgQ;
extern uint32_t gSysUnixTime;

// 刷新时间
static void showTime() {
    u8 buffer[32] = {0};

    time_t unixTime = gSysUnixTime;

    struct tm *tmInfo = localtime(&unixTime);

    strftime(buffer, sizeof(buffer), "%m/%d %H:%M:%S", tmInfo);

    OLED_Display_GB2312_string(0, 0, buffer);
}

// 显示 CPU 使用率
static void showCPU() {
    u8 display_buf[32] = {0};

    sprintf(display_buf, "CPU: %d.%d%%", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100);

    OLED_Display_GB2312_string(0, 6, display_buf);
}

void showDisplayMessage(int p);

// 获取 wifi 模块状态变更消息
void pickMessage() {
    OS_MSG_SIZE msg_size;
    int *p;

    p = (int *) OSQPend(&TempMsgQ,0,OS_OPT_PEND_NON_BLOCKING,&msg_size,NULL,NULL);

    if (*p) {
        showDisplayMessage(*p);
    }
}

static void task() {
    OS_ERR err;

    while (1) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        showTime();

        showCPU();

        pickMessage();
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
