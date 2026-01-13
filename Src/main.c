#include "main.h"
#include <os.h>
#include "bsp.h"

extern UART_HandleTypeDef huart1;
extern uint8_t rx_byte;

void createInitTask();
void GPIO_Init();
void SystemClock_Config();
void MX_USART1_UART_Init();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

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

    createInitTask();

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
