#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT

/*   是否使用CPU名字：DEF_ENABLED或者DEF_DISABLED       */
#define  CPU_CFG_NAME_EN                        DEF_ENABLED



/* CPU名字大小（ASCII字符串形式）   */
#define  CPU_CFG_NAME_SIZE                     16u


/* CPU时间戳功能配置（只能选择其中一个）  */
/*  是否使用32位的时间戳变量：DEF_ENABLED或者DEF_DISABLED             */
#define  CPU_CFG_TS_32_EN                       DEF_ENABLED
/*  是否使用64位的时间戳变量：DEF_ENABLED或者DEF_DISABLED             */
#define  CPU_CFG_TS_64_EN                       DEF_DISABLED
/* *配置CPU时间戳定时器字大小 */
#define  CPU_CFG_TS_TMR_SIZE                    CPU_WORD_SIZE_32



/* 是否使用测量CPU禁用中断的时间  */
#if 0
#define  CPU_CFG_INT_DIS_MEAS_EN
#endif
/* 配置测量的次数*/
#define  CPU_CFG_INT_DIS_MEAS_OVRHD_NBR                    1u


/* 是否使用CPU前导零指令（需要硬件支持，在stm32我们可以使用这个指令）  */
#if 1
#define  CPU_CFG_LEAD_ZEROS_ASM_PRESENT
#endif

#endif