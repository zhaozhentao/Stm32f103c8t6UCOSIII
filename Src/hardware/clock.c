#include "stm32f1xx_hal.h"
#include "error_handle.h"

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
