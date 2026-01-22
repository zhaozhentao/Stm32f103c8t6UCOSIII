#ifndef __OLED_H__

#define __OLED_H__

#include "stm32f1xx_hal.h"

typedef uint8_t u8;
typedef uint32_t u32;

void OLED_Init();

void OLED_ColorTurn(u8 i);

void OLED_DisplayTurn(u8 i);

void OLED_Clear();

void OLED_Display_GB2312_string(u8 x, u8 y, u8 *text);

void OLED_Display_128x64(u8 *dp);

#endif
