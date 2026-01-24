#include <os.h>
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "uart.h"
#include "oled.h"
#include "prio.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull  // 1970 - 1900 差值秒数

#define UART_BUFFER_LENGTH 1024

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
extern char rx_buf[UART_BUFFER_LENGTH];
extern uint32_t gSysUnixTime;
extern OS_MUTEX gTimeMutex;
extern OS_Q TempMsgQ;

void showTemperature(char *temperature);

int process = 0;
OS_TCB UartTaskTCB;
CPU_STK UartTaskStk[LED_TASK_STK_SIZE];

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t * ) & ch, 1, 50);
    return ch;
}

const char *AT = "AT\r\n";
const char *AT_CWJAP = "AT+CWJAP?\r\n";

typedef enum {
    AT_OK,
    AT_ERROR,
    AT_TIMEOUT
} AT_Status;

static void showError() {
    OS_ERR err;
    int i = 0;
    char tmp[17];

    while (i < rx_len) {
        snprintf(tmp, 16, "%-16s", rx_buf + i);
        OLED_Display_GB2312_string(0, 4, tmp);
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
        i += 16;
    }
}

static AT_Status sendATCmd(char *cmd, char *expect, int timeoutSec) {
    OS_ERR err;

    rx_len = 0;
    uart_rx_finished = 0;
    memset(rx_buf, 0, 1024);

    HAL_UART_Transmit(&huart2, (uint8_t *) cmd, strlen(cmd), 50);

    while (timeoutSec-- > 0) {
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_DLY, &err);

        if (!uart_rx_finished) {
            continue;
        }

        if (strstr((char *) rx_buf, expect) != NULL)
            return AT_OK;

        if (strstr((char *) rx_buf, "ERROR") != NULL)
            return AT_ERROR;
    }

    return AT_TIMEOUT;
}

static int findLastNonZeroIndex() {
    // 从最后一个索引（255）开始向前遍历
    for (int i = UART_BUFFER_LENGTH - 1; i >= 0; i--) {
        // 找到第一个非0元素，返回索引
        if (rx_buf[i] != 0) {
            return i;
        }
    }
    // 数组全为0
    return -1;
}

static void sendDisplayMessage(int p) {
    process = p;

    OSQPost(&TempMsgQ, &process, sizeof(process), OS_OPT_POST_FIFO, 0);
}

static AT_Status checkNetwork() {
    sendDisplayMessage(1);
    if (sendATCmd(AT, "OK", 4) != AT_OK) {
        return AT_ERROR;
    }

    sendDisplayMessage(2);
    if (sendATCmd(AT_CWJAP, "CWJAP", 6) == AT_OK) {
        return AT_OK;
    }

    sendDisplayMessage(3);
    if (sendATCmd("AT+CWMODE=1\r\n", "OK", 4) != AT_OK) {
        return AT_ERROR;
    }

    sendDisplayMessage(4);
    if (sendATCmd("AT+CWJAP=\"zhao1003\",\"zhaozhao\"\r\n", "OK", 14) != AT_OK) {
        return AT_ERROR;
    }

    return AT_OK;
}

static void sendQuery() {
    OS_ERR err;
    int timeoutSec = 10;
    unsigned long long secs_since_1900;
    unsigned char packet[48] = {0};

    rx_len = 0;
    uart_rx_finished = 0;
    memset(rx_buf, 0, 256);

    packet[0] = 0x1B;

    HAL_UART_Transmit(&huart2, (uint8_t *) packet, sizeof(packet), 100);

    while (timeoutSec-- > 0) {
        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_DLY, &err);

        if (!uart_rx_finished) {
            continue;
        }

        // 认为接收完成
        if (findLastNonZeroIndex() != 85) {
            continue;
        }

        uart_rx_finished = 0;
        // 一共 86 字节
        // 会收到这样的响应 0D 0A 52 65 63 76 20 34 38 20 62 79 74 65 73 0D 0A 0D 0A 53 45 4E 44 20 4F 4B 0D 0A 0D 0A 2B 49 50 44 2C 34 38 3A 1C 02 00 E7 00 00 04 2A 00 00 00 44 64 6B 19 72 ED 12 19 98 EF F3 04 7B 00 00 00 00 00 00 00 00 ED 12 19 B0 F8 10 43 3D ED 12 19 B0 F8 11 30 25
        secs_since_1900 =
                ((unsigned long long) rx_buf[78] << 24) |
                ((unsigned long long) rx_buf[79] << 16) |
                ((unsigned long long) rx_buf[80] << 8) |
                ((unsigned long long) rx_buf[81]);

        OSMutexPend(&gTimeMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        gSysUnixTime = secs_since_1900 - NTP_TIMESTAMP_DELTA + (8 * 3600);
        OSMutexPost(&gTimeMutex, OS_OPT_POST_NONE, &err);

        sendDisplayMessage(11);

        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        // 清除同步完成
        sendDisplayMessage(9);
        return;
    }
}

/*
 * 检查连接状态 AT+CWJAP?
 * 未连接返回   No AP
 * 已连接返回   +CWJAP:"Yu","5a:41:44:0d:3d:7b",6,-19,0
 */
static void ntpSync() {
    OS_ERR err;

    if (gSysUnixTime != 0) {
        // 已经同步时间
        return;
    }

    if (checkNetwork() != AT_OK) {
        return;
    }

    sendDisplayMessage(10);
    if (sendATCmd("AT+CIPSTART=\"UDP\",\"ntp.aliyun.com\",123\r\n", "CONNECTED", 10) != AT_OK) {
        return;
    }

    sendDisplayMessage(6);
    if (sendATCmd("AT+CIPSEND=48\r\n", "OK", 6) != AT_OK) {
        return;
    }

    sendQuery();

    // udp 连接需要关闭，否则影响后续的 tcp 连接
    sendDisplayMessage(12);
    if (sendATCmd("AT+CIPCLOSE\r\n", "OK", 6) != AT_OK) {
        return;
    }

    OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    sendDisplayMessage(9);
}

static const unsigned char str[] =
        "GET /v1/forecast?latitude=23.0200&longitude=113.1200&current_weather=true&timezone=Asia/Shanghai HTTP/1.1\n"
        "Host: api.open-meteo.com\n"
        "Connection: close\n\n\r\n";

char temperature[16] = {};  // 存放温度

static void parseTemperature() {
    char *cw = strstr(rx_buf, "\"current_weather\"");
    if (cw == NULL) {
        return;
    }

    char *p = strstr(cw, "\"temperature\":");
    if (p == NULL) {
        return;
    }

    p += strlen("\"temperature\":"); // 跳过 "temperature":

    // 拷贝数字直到遇到非数字、小数点、负号
    int i = 0;
    while ((*p == '-' || *p == '+' || *p == '.' || (*p >= '0' && *p <= '9')) && i < 15) {
        temperature[i++] = *p;
        p++;
    }
    temperature[i] = '\0';

    showTemperature(temperature);
}

static void sendWeatherQuery() {
    OS_ERR err;
    int timeoutSec = 10;

    rx_len = 0;
    uart_rx_finished = 0;
    memset(rx_buf, 0, 1024);

    HAL_UART_Transmit(&huart2, str, sizeof(str) - 1, 200);

    while (timeoutSec-- > 0) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        char *found = strstr(rx_buf, "CLOSED");

        if (found == NULL) {
            continue;
        }

        parseTemperature();

        sendDisplayMessage(7);
        return;
    }

    sendDisplayMessage(8);
}

static void weather() {
    OS_ERR err;

    if (strlen(temperature) != 0) {
        return;
    }

    if (checkNetwork() != AT_OK) {
        return;
    }

    // 已经连接 wifi
    sendDisplayMessage(5);
    if (sendATCmd("AT+CIPSTART=\"TCP\",\"api.open-meteo.com\",80\r\n", "OK", 10) != AT_OK) {
        return;
    }

    // 设置请求长度
    sendDisplayMessage(6);
    if (sendATCmd("AT+CIPSEND=150\r\n", ">", 6) != AT_OK) {
        return;
    }

    sendWeatherQuery();

    OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);
    sendDisplayMessage(9);
}

static void task() {
    OS_ERR err;

    // 在 OS 启动后才能开启中断，否则 CPU 统计有问题
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);

    showTemperature("");

    while (1) {
        OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY, &err);

        ntpSync();

        weather();
    }
}

void createTask2() {
    OSTaskCreate((OS_TCB * ) & UartTaskTCB,
                 (CPU_CHAR *) "Uart Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) ESP_TASK_PRIO,
                 (CPU_STK * ) & UartTaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
