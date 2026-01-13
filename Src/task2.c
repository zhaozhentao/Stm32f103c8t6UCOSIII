#include <os.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

#define LED_TASK_STK_SIZE 128

#ifdef __GNUC__
/* 使用 GCC 编译器 */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

extern UART_HandleTypeDef huart1;

OS_TCB UartTaskTCB;
CPU_STK UartTaskStk[LED_TASK_STK_SIZE];

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t * ) & ch, 1, HAL_MAX_DELAY);
    return ch;
}

void UartTask() {
    OS_ERR err;

    while (1) {
//        printf("hello ucos iii\r\n");

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    }
}

void createTask2() {
    OSTaskCreate((OS_TCB * ) & UartTaskTCB,
                 (CPU_CHAR *) "Uart Task",
                 (OS_TASK_PTR) UartTask,
                 (void *) 0,
                 (OS_PRIO) 6,
                 (CPU_STK * ) & UartTaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
