#ifndef __USBD_KEYBOARD_H
#define __USBD_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_hid.h"

typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keycode[6];
} KeyboardReport_TypeDef;

/* 修饰键定义 */
#define KEY_MODIFIER_LEFT_CTRL    0x01
#define KEY_MODIFIER_LEFT_SHIFT   0x02
#define KEY_MODIFIER_LEFT_ALT     0x04
#define KEY_MODIFIER_LEFT_GUI     0x08
#define KEY_MODIFIER_RIGHT_CTRL   0x10
#define KEY_MODIFIER_RIGHT_SHIFT  0x20
#define KEY_MODIFIER_RIGHT_ALT    0x40
#define KEY_MODIFIER_RIGHT_GUI    0x80

/* 按键代码定义 */
#define KEY_NONE                  0x00
#define KEY_A                     0x04
#define KEY_B                     0x05
#define KEY_C                     0x06
#define KEY_D                     0x07
#define KEY_E                     0x08
#define KEY_F                     0x09
#define KEY_G                     0x0A
#define KEY_H                     0x0B
#define KEY_I                     0x0C
#define KEY_J                     0x0D
#define KEY_K                     0x0E
#define KEY_L                     0x0F
#define KEY_M                     0x10
#define KEY_N                     0x11
#define KEY_O                     0x12
#define KEY_P                     0x13
#define KEY_Q                     0x14
#define KEY_R                     0x15
#define KEY_S                     0x16
#define KEY_T                     0x17
#define KEY_U                     0x18
#define KEY_V                     0x19
#define KEY_W                     0x1A
#define KEY_X                     0x1B
#define KEY_Y                     0x1C
#define KEY_Z                     0x1D
#define KEY_1                     0x1E
#define KEY_2                     0x1F
#define KEY_3                     0x20
#define KEY_4                     0x21
#define KEY_5                     0x22
#define KEY_6                     0x23
#define KEY_7                     0x24
#define KEY_8                     0x25
#define KEY_9                     0x26
#define KEY_0                     0x27
#define KEY_ENTER                 0x28
#define KEY_ESC                   0x29
#define KEY_BACKSPACE             0x2A
#define KEY_TAB                   0x2B
#define KEY_SPACE                 0x2C
#define KEY_MINUS                 0x2D
#define KEY_EQUAL                 0x2E
#define KEY_LEFT_BRACKET          0x2F
#define KEY_RIGHT_BRACKET         0x30
#define KEY_BACKSLASH             0x31
#define KEY_SEMICOLON             0x33
#define KEY_QUOTE                 0x34
#define KEY_GRAVE                 0x35
#define KEY_COMMA                 0x36
#define KEY_DOT                   0x37
#define KEY_SLASH                 0x38
#define KEY_CAPS_LOCK             0x39
#define KEY_F1                    0x3A
#define KEY_F2                    0x3B
#define KEY_F3                    0x3C
#define KEY_F4                    0x3D
#define KEY_F5                    0x3E
#define KEY_F6                    0x3F
#define KEY_F7                    0x40
#define KEY_F8                    0x41
#define KEY_F9                    0x42
#define KEY_F10                   0x43
#define KEY_F11                   0x44
#define KEY_F12                   0x45

/* 函数声明 */
uint8_t KEYBOARD_SendKey(USBD_HandleTypeDef *pdev, uint8_t keycode);
uint8_t KEYBOARD_SendKeyWithModifier(USBD_HandleTypeDef *pdev, uint8_t keycode, uint8_t modifier);
uint8_t KEYBOARD_ReleaseAllKeys(USBD_HandleTypeDef *pdev);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_KEYBOARD_H */
