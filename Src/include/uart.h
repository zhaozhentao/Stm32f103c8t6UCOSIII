#ifndef __UART_H__

#define __UART_H__

void MX_USART1_UART_Init();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
