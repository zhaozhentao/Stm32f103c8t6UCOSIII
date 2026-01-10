#include <os.h>
#include "stm32f1xx_hal.h"

#define LED_TASK_STK_SIZE 128

OS_TCB LedTaskTCB;
CPU_STK LedTaskStk[LED_TASK_STK_SIZE];

void LedTask() {
    OS_ERR err;

    while (1) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED 灭
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // LED 亮
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    }
}

void createTask1(void) {
    OSTaskCreate((OS_TCB * ) & LedTaskTCB,
                 (CPU_CHAR *) "LED Task",
                 (OS_TASK_PTR) LedTask,
                 (void *) 0,
                 (OS_PRIO) 5,
                 (CPU_STK * ) & LedTaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
