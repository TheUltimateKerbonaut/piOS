#include <stddef.h>
#include <stdint.h>
 
#include "io/mbox.h"
#include "io/uart.h"
#include "io/atag.h"

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

	createSizeMailbox();
	uint32_t sizeMailbox = sendMailbox(ArmToVC);
	if (sizeMailbox == 0) uart_print("Mailbox error!");
	else
	{
		uart_printi("Width: ");
		uart_dec(mailbox[5]);
		uart_printi("\r\nHeight: ");
		uart_dec(mailbox[6]);
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
	

	uart_print("Finished");

	while (1)  //{ uart_print("Hello, kernel World!\r\n"); }
		uart_putc(uart_getc());
}