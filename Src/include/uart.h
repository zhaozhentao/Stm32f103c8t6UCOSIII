#ifndef __UART_H__

#define __UART_H__

extern UART_HandleTypeDef huart1;
extern uint8_t rx_byte;

void MX_USART1_UART_Init();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
