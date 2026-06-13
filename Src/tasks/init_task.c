#include <os.h>
#include "bsp.h"
#include "oled.h"
#include "prio.h"
#include "usbd_keyboard.h"

#define  APP_TASK_STATUS_STK_SIZE                   128

void createTask1();
void createTask2();
void createTask3();

void createCPUTask();

extern USBD_HandleTypeDef hUsbDeviceFS;

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

//    Mem_Init();

    OSStatTaskCPUUsageInit(&err);

    CPU_IntDisMeasMaxCurReset();

    /* 等待USB枚举完成 */
    OSTimeDly(200, OS_OPT_TIME_DLY, &err);

    /* 测试USB键盘功能 - 发送HELLO */
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_H);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_E);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_L);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_L);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_O);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    
    /* 发送回车键 */
    KEYBOARD_SendKey(&hUsbDeviceFS, KEY_ENTER);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    KEYBOARD_ReleaseAllKeys(&hUsbDeviceFS);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);

    OLED_Init();
    OLED_ColorTurn(0);   //0正常显示，1 反色显示
    OLED_DisplayTurn(0); //0正常显示 1 屏幕翻转显示
    OLED_Clear();

    createTask1();

    createTask2();

    createTask3();

    createCPUTask();

    OSTaskDel(&AppTaskStartTCB, &err);
}

void createInitTask() {
    OS_ERR err;

    OSTaskCreate((OS_TCB * ) & AppTaskStartTCB,
                 (CPU_CHAR *) "App Init Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) INIT_TASK_PRIO,
                 (CPU_STK * ) & AppTaskStatusStk[0],
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_STATUS_STK_SIZE,
                 (OS_MSG_QTY) 5u,
                 (OS_TICK) 0u,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR * ) & err);
}
