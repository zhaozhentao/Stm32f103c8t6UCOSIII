#ifndef  BSP_PRESENT
#define  BSP_PRESENT

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

#include  <stdarg.h>
#include  <stdio.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_ascii.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#include "stm32f1xx_hal.h"

#include  <app_cfg.h>

void BSP_Init(void);

CPU_INT32U BSP_CPU_ClkFreq(void);

#endif                                                          /* End of module include.                               */




