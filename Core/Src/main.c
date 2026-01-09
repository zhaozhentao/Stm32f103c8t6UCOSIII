#include "main.h"

#include <os.h>

#define LED_TASK_STK_SIZE 128
OS_TCB LedTaskTCB;
CPU_STK LedTaskStk[LED_TASK_STK_SIZE];

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // 1. 配置外部高速时钟（HSE）和PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    // 启用外部8MHz晶振
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;     // HSE不分频
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                // 启用PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        // PLL源=HSE
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                // HSE×9=72MHz（核心）
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // 2. 配置系统时钟分频（必须遵守硬件限制）
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // 系统时钟=PLL=72MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // HCLK = 72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // PCLK1 = 36MHz（≤36MHz硬件限制）
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // PCLK2 = 72MHz

    // 3. 应用时钟配置（72MHz下必须设置FLASH_LATENCY_2）
    // ❗ 这是最容易忽略的点，不设置会导致系统不稳定/卡死
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }

    // 4. 关键：更新SystemCoreClock为实际值（72MHz）
    SystemCoreClockUpdate();
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 默认设置为高电平（LED 灭）
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void LedTask(void *p_arg) {
    (void) p_arg;
    OS_ERR err;

    while (1) {
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED 灭
//        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_DLY, &err);
//
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // LED 亮
//        OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_DLY, &err);
    }
}

int main(void) {
    OS_ERR err;

    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
//    HAL_SuspendTick();

    OSInit(&err);
    if (err != OS_ERR_NONE) {
        while (1);
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // LED 亮

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

    OSStart(&err);

//    while (1) {
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // LED 亮
//        HAL_Delay(500);
//
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED 灭
//        HAL_Delay(500);
//    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {

    }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
