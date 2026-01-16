#include <os.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "stm32f1xx_hal.h"
#include "uart.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull  // 1970 - 1900 差值秒数

#define LED_TASK_STK_SIZE 512

#ifdef __GNUC__
/* 使用 GCC 编译器 */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

extern uint8_t uart_rx_finished;
extern uint8_t rx_byte;
extern uint16_t rx_len;
extern uint8_t rx_buf[256];

OS_TCB UartTaskTCB;
CPU_STK UartTaskStk[LED_TASK_STK_SIZE];

typedef uint8_t u8;
typedef uint32_t u32;

void OLED_Init();
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_Clear();
void OLED_Display_GB2312_string(u8 x, u8 y, u8 *text);

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t * ) & ch, 1, 50);
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

    memset(rx_buf, 0, 256);

    HAL_UART_Transmit(&huart2, (uint8_t *) cmd, strlen(cmd), 50);

    while (timeoutSec-- > 0) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        if (!uart_rx_finished) {
            continue;
        }

        if (strstr((char *) rx_buf, expect) != NULL)
            return AT_OK;
        else if (strstr((char *) rx_buf, "ERROR") != NULL)
            return AT_ERROR;
    }

    return AT_TIMEOUT;
}

static void sendQuery() {
    int timeoutSec = 10;
    OS_ERR err;
    unsigned long long secs_since_1900;
    unsigned char packet[48] = {0};
    u8 display_buf[32] = {0};

    rx_len = 0;
    uart_rx_finished = 0;
    packet[0] = 0x1B;

    memset(rx_buf, 0, 256);
    HAL_UART_Transmit(&huart2, (uint8_t *) packet, sizeof(packet), 100);

    while (timeoutSec-- > 0) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        if (!uart_rx_finished) {
            continue;
        }

        // 接收到换行，认为接收完成
        uart_rx_finished = 0;
        // 一共 86 字节
        // 会收到这样的响应 0D 0A 52 65 63 76 20 34 38 20 62 79 74 65 73 0D 0A 0D 0A 53 45 4E 44 20 4F 4B 0D 0A 0D 0A 2B 49 50 44 2C 34 38 3A 1C 02 00 E7 00 00 04 2A 00 00 00 44 64 6B 19 72 ED 12 19 98 EF F3 04 7B 00 00 00 00 00 00 00 00 ED 12 19 B0 F8 10 43 3D ED 12 19 B0 F8 11 30 25
        secs_since_1900 =
                ((unsigned long long) rx_buf[78] << 24) |
                ((unsigned long long) rx_buf[79] << 16) |
                ((unsigned long long) rx_buf[80] << 8) |
                ((unsigned long long) rx_buf[81]);

        time_t unix_time = secs_since_1900 - NTP_TIMESTAMP_DELTA + (8 * 3600);

        struct tm *tm_info = localtime(&unix_time);

        sprintf(display_buf, "%s", asctime(tm_info));

        OLED_Display_GB2312_string(0, 0, display_buf);

        return;
    }
}

static void wifiInit() {
    if (sendATCmd("AT\r\n", "OK", 2) != AT_OK) {
        printf("AT ERROR\r\n");
        return;
    }

    if (sendATCmd("AT+CWMODE=1\r\n", "OK", 2) != AT_OK) {
        printf("CWMODE ERROR\r\n");
        return;
    }

    if (sendATCmd("AT+CWJAP=\"Yu\",\"qwertyuiop\"\r\n", "OK", 10) != AT_OK) {
        printf("CWJAP ERROR\r\n");
        return;
    }

    if (sendATCmd("AT+CIPSTART=\"UDP\",\"ntp.aliyun.com\",123\r\n", "CONNECTED", 5) != AT_OK) {
        printf("aliyun ERROR\r\n");
        return;
    }

    if (sendATCmd("AT+CIPSEND=48\r\n", "OK", 5) != AT_OK) {
        printf("CIPSEND ERROR");
        return;
    }

    sendQuery();
}

static void task() {
    OS_ERR err;

    OLED_Init();
    OLED_ColorTurn(0);   //0正常显示，1 反色显示
    OLED_DisplayTurn(0); //0正常显示 1 屏幕翻转显示
    OLED_Clear();

    while (1) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        wifiInit();
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
