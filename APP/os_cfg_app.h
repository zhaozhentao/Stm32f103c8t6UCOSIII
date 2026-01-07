#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H


/* --------------------- MISCELLANEOUS ------------------ */
#define  OS_CFG_MSG_POOL_SIZE            100u/* 支持的最大消息数量 */
#define  OS_CFG_ISR_STK_SIZE             128u/*ISR栈的大小 */
#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY  10u/*检查栈的剩余大小（百分百形式，
                        此处是10%）*/


/* ---------------------- 空闲任务 --------------------- */
#define  OS_CFG_IDLE_TASK_STK_SIZE       128u/* 空闲任务栈大小    */


/* ------------------ 中断处理任务------------------ */
#define  OS_CFG_INT_Q_SIZE                10u/*中断处理任务队列大小  */
#define  OS_CFG_INT_Q_TASK_STK_SIZE      128u/* 中断处理任务的栈大小*/


/* ------------------- 统计任务------------------- */
#define  OS_CFG_STAT_TASK_PRIO            11u/* 统计任务的优先级  */
#define  OS_CFG_STAT_TASK_RATE_HZ         10u/* 统计任务的指向频率（10HZ）*/
#define  OS_CFG_STAT_TASK_STK_SIZE       128u/*统计任务的栈大小*/


/* ------------------------ 时钟节拍任务 ----------------------- */
#define  OS_CFG_TICK_RATE_HZ       1000u/*系统的时钟节拍(一般为10 到 1000 Hz) */
#define  OS_CFG_TICK_TASK_PRIO            1u/*时钟节拍任务的优先级    */
#define  OS_CFG_TICK_TASK_STK_SIZE       128u/* 时钟节拍任务的栈大小*/
#define  OS_CFG_TICK_WHEEL_SIZE           17u/* 时钟节拍任务的列表大小 */


/* ----------------------- 定时器任务 ----------------------- */
#define  OS_CFG_TMR_TASK_PRIO          11u/*定时器任务的优先级  */
#define  OS_CFG_TMR_TASK_RATE_HZ        10u/* 定时器频率（10 Hz是典型值） */
#define  OS_CFG_TMR_TASK_STK_SIZE      128u/* 定时器任务的栈大小    */
#define  OS_CFG_TMR_WHEEL_SIZE          17u/*定时器任务的列表大小  */

#endif