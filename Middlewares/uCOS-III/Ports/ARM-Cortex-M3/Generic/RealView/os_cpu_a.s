.syntax unified
.cpu cortex-m3
.thumb

.global OSStartHighRdy
.global OS_CPU_PendSVHandler

.extern OSRunning
.extern OSPrioCur
.extern OSPrioHighRdy
.extern OSTCBCurPtr
.extern OSTCBHighRdyPtr
.extern OSIntExit
.extern OSTaskSwHook
.extern OS_CPU_ExceptStkBase

/* -------------------------------------------------------------------------- */
/* EQU constants                                                             */
/* -------------------------------------------------------------------------- */

.equ NVIC_INT_CTRL,   0xE000ED04
.equ NVIC_SYSPRI14,   0xE000ED22
.equ NVIC_PENDSV_PRI, 0xFF
.equ NVIC_PENDSVSET,  0x10000000

/* -------------------------------------------------------------------------- */
/* OSStartHighRdy                                                            */
/* -------------------------------------------------------------------------- */

.section .text.OSStartHighRdy, "ax", %progbits
.align 2
.thumb_func
OSStartHighRdy:
    /* Set PendSV priority lowest */
    ldr     r0, =NVIC_SYSPRI14
    ldr     r1, =NVIC_PENDSV_PRI
    strb    r1, [r0]

    /* PSP = 0 so PendSV knows it's first switch */
    movs    r0, #0
    msr     psp, r0

    /* MSP = OS_CPU_ExceptStkBase */
    ldr     r0, =OS_CPU_ExceptStkBase
    ldr     r1, [r0]
    msr     msp, r1

    /* Trigger PendSV */
    ldr     r0, =NVIC_INT_CTRL
    ldr     r1, =NVIC_PENDSVSET
    str     r1, [r0]

    cpsie   i

OSStartHang:
    b       OSStartHang

/* -------------------------------------------------------------------------- */
/* PendSV Handler                                                            */
/* -------------------------------------------------------------------------- */

.section .text.OS_CPU_PendSVHandler, "ax", %progbits
.align 2
.thumb_func
OS_CPU_PendSVHandler:
    cpsid   i

    /* R0 = PSP */
    mrs     r0, psp
    cbz     r0, no_save           /* First context switch → skip saving R4~R11 */

    /* Save R4-R11 on task stack */
    subs    r0, r0, #0x20
    stmia   r0, {r4-r11}

    /* Save new stack pointer into TCB */
    ldr     r1, =OSTCBCurPtr
    ldr     r1, [r1]
    str     r0, [r1]

no_save:
    /* Call OSTaskSwHook */
    push    {lr}
    ldr     r0, =OSTaskSwHook
    blx     r0
    pop     {lr}

    /* OSPrioCur = OSPrioHighRdy */
    ldr     r0, =OSPrioCur
    ldr     r1, =OSPrioHighRdy
    ldrb    r2, [r1]
    strb    r2, [r0]

    /* OSTCBCurPtr = OSTCBHighRdyPtr */
    ldr     r0, =OSTCBCurPtr
    ldr     r1, =OSTCBHighRdyPtr
    ldr     r2, [r1]
    str     r2, [r0]

    /* Load new task stack pointer */
    ldr     r0, [r2]

    /* Restore R4-R11 */
    ldmia   r0, {r4-r11}
    adds    r0, r0, #0x20
    msr     psp, r0

    /* Ensure exception return uses PSP */
    orr     lr, lr, #0x04

    cpsie   i
    bx      lr
