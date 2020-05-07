.section ".text.boot"

.global _start

_start:

    //b 0x1234560

    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // cpu id > 0, stop
1:  
    wfe
    b       1b
2:  // cpu id == 0

    // set stack before our code
    ldr     x1, =__EL2_stack_core0
    mov     sp, x1

    // clear bss
    ldr     x1, =__bss_start__
    ldr     w2, =__bss_size__
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b

    b onException

    // jump to C code, should not return
4:  
    b jumpToKernel

.global jumpToKernel
jumpToKernel:
    mov x0, 0x100
    bl      kernel_main
    // for failsafe, halt this core too
    b       1b

.global setupStack
setupStack:
    ldr x1, =__stack_start_core1__
    mov sp, x1
    b initCore
    