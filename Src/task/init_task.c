#include <os.h>
#include "bsp.h"
#include "oled.h"
#include "prio.h"

#define  APP_TASK_STATUS_STK_SIZE                   128

void createLedTask();

void createNTPTask();

void createDisplayTask();

void createTimerTask();

static OS_TCB TaskTCB;
static CPU_STK TaskStk[APP_TASK_STATUS_STK_SIZE];

static void task() {
    OS_ERR err;
    CPU_INT32U cpu_clk_freq;
    CPU_INT32U cnts;

    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U) OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);

//    Mem_Init();

    OSStatTaskCPUUsageInit(&err);

    CPU_IntDisMeasMaxCurReset();

    OLED_Init();
    OLED_ColorTurn(0);   //0正常显示，1 反色显示
    OLED_DisplayTurn(0); //0正常显示 1 屏幕翻转显示
    OLED_Clear();

    createLedTask();

    createNTPTask();

    createDisplayTask();

    createTimerTask();

    OSTaskDel(&TaskTCB, &err);
}

void createInitTask() {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & TaskTCB,
                 (CPU_CHAR *) "Init Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) INIT_TASK_PRIO,
                 (CPU_STK * ) & TaskStk[0],
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR * ) & err);
}
