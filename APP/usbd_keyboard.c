#include "usbd_keyboard.h"
#include "usbd_conf.h"
#include <string.h>

static KeyboardReport_TypeDef keyboard_report;

/**
  * @brief  发送单个按键
  * @param  pdev: USB设备句柄
  * @param  keycode: 按键代码
  * @retval USBD状态
  */
uint8_t KEYBOARD_SendKey(USBD_HandleTypeDef *pdev, uint8_t keycode)
{
  memset(&keyboard_report, 0, sizeof(KeyboardReport_TypeDef));
  keyboard_report.keycode[0] = keycode;
  
  return USBD_HID_SendReport(pdev, (uint8_t*)&keyboard_report, sizeof(KeyboardReport_TypeDef));
}

/**
  * @brief  发送带修饰键的按键（如Ctrl+A）
  * @param  pdev: USB设备句柄
  * @param  keycode: 按键代码
  * @param  modifier: 修饰键（KEY_MODIFIER_*）
  * @retval USBD状态
  */
uint8_t KEYBOARD_SendKeyWithModifier(USBD_HandleTypeDef *pdev, uint8_t keycode, uint8_t modifier)
{
  memset(&keyboard_report, 0, sizeof(KeyboardReport_TypeDef));
  keyboard_report.modifiers = modifier;
  keyboard_report.keycode[0] = keycode;
  
  return USBD_HID_SendReport(pdev, (uint8_t*)&keyboard_report, sizeof(KeyboardReport_TypeDef));
}

/**
  * @brief  释放所有按键
  * @param  pdev: USB设备句柄
  * @retval USBD状态
  */
uint8_t KEYBOARD_ReleaseAllKeys(USBD_HandleTypeDef *pdev)
{
  memset(&keyboard_report, 0, sizeof(KeyboardReport_TypeDef));
  
  return USBD_HID_SendReport(pdev, (uint8_t*)&keyboard_report, sizeof(KeyboardReport_TypeDef));
}
