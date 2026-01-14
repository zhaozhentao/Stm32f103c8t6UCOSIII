#include <os.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

#define LED_TASK_STK_SIZE 128

#ifdef __GNUC__
/* 使用 GCC 编译器 */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

extern UART_HandleTypeDef huart1;
extern uint8_t uart_rx_finished;
extern uint8_t rx_byte;
extern uint16_t rx_len;
extern uint8_t rx_buf[256];

OS_TCB UartTaskTCB;
CPU_STK UartTaskStk[LED_TASK_STK_SIZE];

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t * ) & ch, 1, HAL_MAX_DELAY);
    return ch;
}

typedef enum {
    AT_OK,
    AT_ERROR,
    AT_TIMEOUT
} AT_Status;

static AT_Status sendATCmd(char *cmd, char *expect, int timeoutSec) {
    OS_ERR err;
    rx_len = 0;
    uart_rx_finished = 0;

    HAL_UART_Transmit(&huart1, (uint8_t *) cmd, strlen(cmd), 100);

    while (timeoutSec-- > 0) {
        if (uart_rx_finished) {
            uart_rx_finished = 0;
            rx_buf[rx_len] = '\0';

            if (strstr((char *) rx_buf, expect) != NULL)
                return AT_OK;
            else if (strstr((char *) rx_buf, "ERROR") != NULL)
                return AT_ERROR;
        }

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    }

    return AT_TIMEOUT;
}

static void wifiInit() {
    if (sendATCmd("AT\r\n", "OK", 10) != AT_OK)
        return;

    if (sendATCmd("AT+CWMODE=1\r\n", "OK", 10) != AT_OK)
        return;

    if (sendATCmd("AT+CWJAP=\"ssid\",\"pwd\"\r\n", "WIFI CONNECTED", 10) != AT_OK)
        return;

    if (sendATCmd("AT+CIPMUX=0\r\n", "OK", 10) != AT_OK)
        return;
}

static void task() {
    OS_ERR err;

    while (1) {
        wifiInit();

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    }
}

void createTask2() {
    OSTaskCreate((OS_TCB * ) & UartTaskTCB,
                 (CPU_CHAR *) "Uart Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) 8,
                 (CPU_STK * ) & UartTaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
