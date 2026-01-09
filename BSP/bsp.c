#include "stm32f1xx_hal.h"  // 根据芯片型号选择
#include "bsp.h"

CPU_INT32U  BSP_CPU_ClkFreq_MHz;

void BSP_Init(void)
{
    // 初始化 HAL 库
    HAL_Init();

    // 初始化 LED
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13;    // 以 PC13 为例
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

CPU_INT32U BSP_CPU_ClkFreq(void)
{
    // HAL 提供获取 HCLK 的函数
    return HAL_RCC_GetHCLKFreq();
}

#if ((APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN == 1))
void OSProbe_TmrInit(void)
{
    // 使用 DWT 初始化
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

CPU_INT32U OSProbe_TmrRd(void)
{
    return DWT->CYCCNT;
}
#endif

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrInit(void)
{
    // DWT 初始化
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    CPU_TS_TmrFreqSet(BSP_CPU_ClkFreq());
}

CPU_TS_TMR CPU_TS_TmrRd(void)
{
    return DWT->CYCCNT;
}
#endif
