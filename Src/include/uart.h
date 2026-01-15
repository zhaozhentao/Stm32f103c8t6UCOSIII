#ifndef __UART_H__

#define __UART_H__

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t rx_byte;

void MX_USART1_UART_Init();
void MX_USART2_UART_Init();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
