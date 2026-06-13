#include "main.h"
#include <os.h>
#include "gpio.h"
#include "uart.h"
#include "clock.h"

PCD_HandleTypeDef hpcd_USB_FS;

static void MX_USB_PCD_Init(void) {
    /* USER CODE BEGIN USB_Init 0 */

    /* USER CODE END USB_Init 0 */

    /* USER CODE BEGIN USB_Init 1 */

    /* USER CODE END USB_Init 1 */
    hpcd_USB_FS.Instance = USB;
    hpcd_USB_FS.Init.dev_endpoints = 8;
    hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_FS.Init.lpm_enable = DISABLE;
    hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USB_Init 2 */

    /* USER CODE END USB_Init 2 */
}

void createInitTask();

void MX_SPI1_Init();

OS_MUTEX gTimeMutex;

OS_MUTEX spiMutex;

OS_Q TempMsgQ;

int main(void) {
    OS_ERR err;

    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_SPI1_Init();
    MX_USB_PCD_Init();

    OSInit(&err);

    if (err != OS_ERR_NONE) {
        while (1);
    }

    OSMutexCreate(&gTimeMutex, "Time Mutex", &err);

    OSMutexCreate(&spiMutex, "SPI Mutex", &err);

    OSQCreate(&TempMsgQ, "Msg Queue", 5, &err);

    createInitTask();

    OSStart(&err);

    while (1);
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
