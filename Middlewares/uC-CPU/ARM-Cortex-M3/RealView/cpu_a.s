.syntax unified
.cpu cortex-m3
.thumb

/* ------------------------------------------------------------- */
/* Global symbols                                                 */
/* ------------------------------------------------------------- */

.global CPU_IntDis
.global CPU_IntEn

.global CPU_SR_Save
.global CPU_SR_Restore

.global CPU_WaitForInt
.global CPU_WaitForExcept

.global CPU_CntLeadZeros
.global CPU_CntTrailZeros
.global CPU_RevBits

/* ------------------------------------------------------------- */
/* Code section                                                   */
/* ------------------------------------------------------------- */

.section .text, "ax", %progbits
.align 2

/* ------------------------------------------------------------- */
/* CPU_IntDis / CPU_IntEn                                        */
/* ------------------------------------------------------------- */
.thumb_func
CPU_IntDis:
    cpsid   i
    bx      lr

.thumb_func
CPU_IntEn:
    cpsie   i
    bx      lr


/* ------------------------------------------------------------- */
/* CPU_SR_Save / CPU_SR_Restore                                  */
/* ------------------------------------------------------------- */
.thumb_func
CPU_SR_Save:
    mrs     r0, primask      /* Save old state */
    cpsid   i                /* Disable interrupts */
    bx      lr

.thumb_func
CPU_SR_Restore:
    msr     primask, r0      /* Restore old PRIMASK */
    bx      lr


/* ------------------------------------------------------------- */
/* CPU_WaitForInt / CPU_WaitForExcept                            */
/* ------------------------------------------------------------- */
.thumb_func
CPU_WaitForInt:
    wfi
    bx      lr

.thumb_func
CPU_WaitForExcept:
    wfe
    bx      lr


/* ------------------------------------------------------------- */
/* CPU_CntLeadZeros                                              */
/* ------------------------------------------------------------- */
.thumb_func
CPU_CntLeadZeros:
    clz     r0, r0
    bx      lr


/* ------------------------------------------------------------- */
/* CPU_CntTrailZeros                                             */
/* ------------------------------------------------------------- */
.thumb_func
CPU_CntTrailZeros:
    rbit    r0, r0
    clz     r0, r0
    bx      lr


/* ------------------------------------------------------------- */
/* CPU_RevBits                                                   */
/* ------------------------------------------------------------- */
.thumb_func
CPU_RevBits:
    rbit    r0, r0
    bx      lr
