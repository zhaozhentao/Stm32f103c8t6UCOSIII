#include "stm32f1xx_hal.h"
#include "error_handle.h"

UART_HandleTypeDef huart1;
uint8_t rx_byte;
uint8_t rx_buf[256];
uint16_t rx_len = 0;
uint8_t uart_rx_finished = 0;

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
    if (huart->Instance != USART1) {
        return;
    }

    rx_buf[rx_len++] = rx_byte;

    if (rx_len >= sizeof(rx_buf)) rx_len = 0;

    // 如果遇到换行符，认为一条返回结束
    if (rx_byte == '\n') {
        uart_rx_finished = 1;
    }

    HAL_UART_Receive_IT(&huart1, &rx_byte, 1); // 继续接收
}

