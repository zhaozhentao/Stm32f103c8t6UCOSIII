#ifndef OS_CFG_H
#define OS_CFG_H


/* --- 其他配置 --- */
#define OS_CFG_APP_HOOKS_EN             1u/* 是否使用钩子函数     */
#define OS_CFG_ARG_CHK_EN               1u/* 是否使用参数检查     */
#define OS_CFG_CALLED_FROM_ISR_CHK_EN   1u/* 是否使用中断调用检查 */
#define OS_CFG_DBG_EN                   1u/* 是否使用debug        */
#define OS_CFG_ISR_POST_DEFERRED_EN     1u/* 是否使用中断延迟post操作*/
#define OS_CFG_OBJ_TYPE_CHK_EN          1u/* 是否使用对象类型检查   */
#define OS_CFG_TS_EN                    1u/*是否使用时间戳     */

#define OS_CFG_PEND_MULTI_EN            1u/*是否使用支持多个任务pend操作*/

#define OS_CFG_PRIO_MAX                32u/*定义任务的最大优先级 */

#define OS_CFG_SCHED_LOCK_TIME_MEAS_EN  1u/*是否使用支持测量调度器锁定时间 */
#define OS_CFG_SCHED_ROUND_ROBIN_EN     1u/* 是否支持循环调度         */
#define OS_CFG_STK_SIZE_MIN            64u/* 最小的任务栈大小        */


/* ---------- 事件标志位---------- */
#define OS_CFG_FLAG_EN                  1u/*是否使用事件标志位    */
#define OS_CFG_FLAG_DEL_EN                  1u/*是否包含OSFlagDel()的代码 */
#define OS_CFG_FLAG_MODE_CLR_EN         1u/*是否包含清除事件标志位的代码*/
#define OS_CFG_FLAG_PEND_ABORT_EN       1u/*是否包含OSFlagPendAbort()的代码*/


/* --------- 内存管理 --- */
#define OS_CFG_MEM_EN                   1u/* 是否使用内存管理         */


/* -------- 互斥量 ----- */
#define OS_CFG_MUTEX_EN                 1u/*是否使用互斥量 */
#define OS_CFG_MUTEX_DEL_EN             1u/*是否包含OSMutexDel()的代码*/
#define OS_CFG_MUTEX_PEND_ABORT_EN      1u/*是否包含OSMutexPendAbort()的代码*/


/* ------- 消息队列--------------- */
#define OS_CFG_Q_EN                     1u/* 是否使用消息队列       */
#define OS_CFG_Q_DEL_EN                 1u/* 是否包含OSQDel()的代码 */
#define OS_CFG_Q_FLUSH_EN               1u/* 是否包含OSQFlush()的代码 */
#define OS_CFG_Q_PEND_ABORT_EN          1u/* 是否包含OSQPendAbort()的代码*/


/* -------------- 信号量 --------- */
#define OS_CFG_SEM_EN                   1u/*是否使用信号量  */
#define OS_CFG_SEM_DEL_EN               1u/*是否包含OSSemDel()的代码*/
#define OS_CFG_SEM_PEND_ABORT_EN        1u/*是否包含OSSemPendAbort()的代码*/
#define OS_CFG_SEM_SET_EN               1u/*是否包含OSSemSet()的代码  */


/* ----------- 任务管理 -------------- */
#define OS_CFG_STAT_TASK_EN             1u/* 是否使用任务统计功能 */
#define OS_CFG_STAT_TASK_STK_CHK_EN     1u/* 从统计任务中检查任务栈 */

#define OS_CFG_TASK_CHANGE_PRIO_EN      1u/* 是否包含OSTaskChangePrio()的代码*/
#define OS_CFG_TASK_DEL_EN              1u/* 是否包含OSTaskDel()的代码*/
#define OS_CFG_TASK_Q_EN                1u/*是否包含OSTaskQXXXX()的代码*/
#define OS_CFG_TASK_Q_PEND_ABORT_EN     1u/* 是否包含OSTaskQPendAbort()的代码 */
#define OS_CFG_TASK_PROFILE_EN          1u/* 是否在OS_TCB中包含变量以进行性能分析 */
#define OS_CFG_TASK_REG_TBL_SIZE     1u/*任务特定寄存器的数量  */
#define OS_CFG_TASK_SEM_PEND_ABORT_EN   1u/* 是否包含OSTaskSemPendAbort()的代码 */
#define OS_CFG_TASK_SUSPEND_EN       1u/*是否包含OSTaskSuspend()和
                        OSTaskResume()的代码*/

/* ------- 时间管理 ------- */
#define OS_CFG_TIME_DLY_HMSM_EN      1u/*是否包含OSTimeDlyHMSM()的代码*/
#define OS_CFG_TIME_DLY_RESUME_EN   1u/*是否包含OSTimeDlyResume()的代码*/


/* ---------- 定时器管理 ------- */
#define OS_CFG_TMR_EN                   1u/* 是否使用定时器        */
#define OS_CFG_TMR_DEL_EN               1u/* 是否支持OSTmrDel()  */

#endif