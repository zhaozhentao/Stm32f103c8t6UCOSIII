#include <os.h>
#include <stdio.h>
#include "bsp.h"
#include "stm32f1xx_hal.h"

#define  APP_TASK_STATUS_PRIO                       2
#define  APP_TASK_STATUS_STK_SIZE                   128

static OS_TCB AppTaskStartTCB;

static CPU_STK AppTaskStatusStk[APP_TASK_STATUS_STK_SIZE];

static void task() {
    OS_ERR err;
    CPU_INT32U cpu_clk_freq;
    CPU_INT32U cnts;

    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U) OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);

    Mem_Init();

    OSStatTaskCPUUsageInit(&err);

    printf("OSStatTaskCPUUsageInit\r\n");

    CPU_IntDisMeasMaxCurReset();

    OSTaskDel ( & AppTaskStartTCB, & err );
}

void createInitTask() {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & AppTaskStartTCB,
                 (CPU_CHAR *) "App Init Task",
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
