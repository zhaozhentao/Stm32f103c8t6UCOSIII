#include <os.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "uart.h"
#include "prio.h"

#define LED_TASK_STK_SIZE 128

OS_TCB LedTaskTCB;
CPU_STK LedTaskStk[LED_TASK_STK_SIZE];

static void task() {
    const int MAX_BRIGHTNESS = 10;  // 最大亮度等级
    OS_ERR err;
    int brightness = 0;             // 当前亮度
    int direction = 1;              // 亮度变化方向: 1=增加, -1=减少
    int counter = 0;                // 计数器，控制亮度变化频率

    while (1) {
        // 设置LED亮度（通过GPIO模拟PWM效果）
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // LED亮
        OSTimeDlyHMSM(0, 0, 0, brightness, OS_OPT_TIME_DLY, &err);

        // 熄灭LED，形成呼吸效果
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED灭
        OSTimeDlyHMSM(0, 0, 0, MAX_BRIGHTNESS - brightness, OS_OPT_TIME_DLY, &err);

        // 每隔几次循环才更新一次亮度，使变化更缓慢
        if (counter++ < 4) {  // 每两次循环才改变一次亮度
            continue;
        }

        brightness += direction;
        counter = 0;

        // 边界检测和方向切换
        if (brightness >= MAX_BRIGHTNESS) {
            direction = -1;  // 开始变暗
        } else if (brightness <= 1) {
            direction = 1;   // 开始变亮
        }
    }
}

void createTask1(void) {
    OSTaskCreate((OS_TCB * ) & LedTaskTCB,
                 (CPU_CHAR *) "LED Task",
                 (OS_TASK_PTR) task,
                 (void *) 0,
                 (OS_PRIO) PWM_TASK_PRIO,
                 (CPU_STK * ) & LedTaskStk[0],
                 (CPU_STK_SIZE)(LED_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) LED_TASK_STK_SIZE,
                 (OS_MSG_QTY) 0,
                 (OS_TICK) 0,
                 (void *) 0,
                 (OS_OPT)(OS_OPT_TASK_STK_CLR),
                 (OS_ERR *) 0);
}
