#include <stddef.h>
#include <stdint.h>
 
#include "mbox.h"
#include "io/uart.h"
#include "io/atag.h"
#include "io/clock.h"

#include "gfx/framebuffer.h"

#include "multitask/core.h"

#define RASPBERRY_PI 3

// 32-bit: void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
void kernel_main(uint64_t atag_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
	uart_init(RASPBERRY_PI);
	
	uart_print("===================");
	uart_print(" Welcome to Pi OS!");
	uart_print("===================");
	uart_print("");	

	createSerialMailbox();
	uint32_t serialMailbox = sendMailbox(ArmToVC);
	if (serialMailbox == 0) uart_print("Mailbox error!");
	else
	{
		uart_printi("Serial number: 0x");
		uart_hex(mailbox[6], 0);
		uart_hex(mailbox[5], 0);
		uart_print("");
	}
	
	uart_print("");

	
	uart_printi("Loading ATAGs from address ");
	uart_hex(atag_ptr32, 1);
	if (populateFromAtag((struct atag*)atag_ptr32) != 0) uart_print("Error!");
	else
	{
		uart_printi("Memory size: ");
		uart_hex(getMemorySize((struct atag*)atag_ptr32), 1);
		uart_printi("Page size: ");
		uart_dec(getPageSize((struct atag*)atag_ptr32));
		uart_print("\r\n");
	}

	int framebufferStatus = initFramebuffer();
	if (framebufferStatus != 0) 
	{
		uart_print("\r\nCritical error with framebuffer!");
		uart_printi("Error code: ");
		uart_hex(framebufferStatus, 1);
	}

	*(uint32_t*)0xE0 = &setupStack;
	sendCoreEvent();
	//*(uint32_t*)0xE8 = &multicoreFunction2;

	delay(100000);

	int jeff = 0;
	for (int i = 0; i < 1000000000; ++i) { jeff = bob; }
	uart_dec(jeff);
	//	uart_putc(uart_getc());
}