#include "main.h"
#include <os.h>

UART_HandleTypeDef huart1;
uint8_t rx_byte;

void createTask1();

void createTask2();

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
    __HAL_RCC_GPIOA_CLK_ENABLE();

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

void MX_USART1_UART_Init(void) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */
    /* USER CODE END USART1_Init 2 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // ⭐ 接收到 1 字节，做你想做的事
        HAL_UART_Transmit(&huart1, &rx_byte, 1, 10);  // 回显

        if (rx_byte == 'w') {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        } else {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        }

        // ⭐ 重新打开中断接收，否则只能接收一次
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

int main(void) {
    OS_ERR err;

    HAL_Init();
    SystemClock_Config();

    GPIO_Init();
    MX_USART1_UART_Init();
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

    OSInit(&err);
    if (err != OS_ERR_NONE) {
        while (1);
    }

    createTask1();

    createTask2();

    OSStart(&err);

    while (1);
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
