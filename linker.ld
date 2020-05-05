ENTRY(_start)
 
SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x80000;
    /* For AArch64, use . = 0x80000; */
    __start = .;

    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    __data_end = .;
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;

    .stack0 :
    {
        . = ALIGN(16); 		/* Stack must always be aligned to 8 byte boundary AAPCS64 call standard */
        __stack_start__core0 = .;	/* Label in case we want address of stack core 1 section start */
        . = . + 512;   		/* HYP stack size core 1 */
    	__HYP_stack_core0 = .;
        . = . + 1024;     		/* IRQ stack size core 1 */
        __IRQ_stack_core0 = .;
        . = . + 1024;     		/* FIQ stack size core 1 */
        __FIQ_stack_core0 = .;
        . = . + 4096;    		/* SVC stack size core 1 */
        __SVC_stack_core0 = .;
        __stack_end__core0 = .;  	/* Label in case we want address of stack core 1 section end */
   	 }
    . = ALIGN(4096); /* align to page size */

    .stack1 :
    {
        . = ALIGN(16); 		/* Stack must always be aligned to 16 byte boundary AAPCS64 call standard */
        __stack_start__core1 = .;	/* Label in case we want address of stack core 1 section start */
        . = . + 512;   		/* HYP stack size core 1 */
    	__HYP_stack_core1 = .;
        . = . + 1024;     		/* IRQ stack size core 1 */
        __IRQ_stack_core1 = .;
        . = . + 1024;     		/* FIQ stack size core 1 */
        __FIQ_stack_core1 = .;
        . = . + 4096;    		/* SVC stack size core 1 */
        __SVC_stack_core1 = .;
        __stack_end__core1 = .;  	/* Label in case we want address of stack core 1 section end */
   	 }
    . = ALIGN(4096); /* align to page size */

    __end = .;
}