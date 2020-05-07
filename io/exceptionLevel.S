.section ".text"

#include "exceptionRegisters.h"

.global getExceptionLevel
getExceptionLevel:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret


.global onException
onException:

    ldr x0, =SCTLR_VALUE_MMU_DISABLED // Binary flags disabling cache and MMU, and using little-endian
    msr sctlr_el1, x0 // Apply for all levels higher and equal to el1

    ldr x0, =HCR_VALUE // Used for hypervisor, but also controls execution state
    msr hcr_el2, x0 // Either AArch64 or AArch32

    //ldr x0, =SCR_VALUE // Responsible for configuring security settings. 
    //msr scr_el3, x0 // Eg whether all lower levels are executed in a secure or nonsecure state. (in this case, AArch64, and nonsecure)

    mov x0, #0x3c4 // Contains processor state that will be restored after we
    msr spsr_el2, x0  // execute ret. This should be set automatically, but we can overwrite certain bits to change exception level.
    // We mask all interrupts and use the EL1 dedicated stack pointer (rather than EL0's one)

    adr x0, 5f // The address we will return to after eret
    msr elr_el2, x0 // We set this to the entrypoint for EL1 code

    eret

5:
    ldr     x1, =__EL1_stack_core0
    mov     sp, x1
6:
    mov     sp, x1
    ldr     x1, =__bss_start__
    ldr     w2, =__bss_size__
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b
4:
    b jumpToKernel

/*
        // running at EL2?
jeff:  
    cmp     x0, #4
    beq     jeff
    msr     sp_el1, x1
    // enable CNTP for EL1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov     x0, #(1 << 31)      // AArch64
    orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
    msr     hcr_el2, x0
    mrs     x0, hcr_el2
    // change execution level to EL1
    mov     x2, #0x3c4
    msr     spsr_el2, x2
    adr     x2, 5f
    msr     elr_el2, x2
    eret

5:
    mov     sp, x1
    ldr     x1, =__bss_start__
    ldr     w2, =__bss_size__
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b
4:
    bl kernel_main
    */