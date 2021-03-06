#include "uart.h"

#include <stddef.h>
#include <stdint.h>
 
#include "clock.h"

#include "../multitask/lock.h"

typedef int lock;
lock uartLock;

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t*)reg;
}
 
// A Mailbox message with set clock rate of PL011 to 3MHz tag
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 4000000, 0 ,0
};
 
void uart_init(uint8_t raspi)
{
	// Disable UART0.
	mmio_write(UART0_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.
 
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write(GPPUD, 0x00000000);
	delay(150);
 
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
 
	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write(GPPUDCLK0, 0x00000000);
 
	// Clear pending interrupts.
	mmio_write(UART0_ICR, 0x7FF);
 
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// Baud = 115200.
 
	// For Raspi3 and 4 the UART_CLOCK is system-clock dependent by default.
	// Set it to 3Mhz so that we can consistently set the baud rate
	if (raspi >= 3) {
		// UART_CLOCK = 40000000;
		unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
		// wait until we can talk to the VC
		while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
		// send our message to property channel and wait for the response
		mmio_write(MBOX_WRITE, r);
		while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r ) { }
	}
 
	// Divider = 4000000 / (16 * 115200) = 2.170 = ~2.
	mmio_write(UART0_IBRD, 2);
	// Fractional part register = (.170 * 64) + 0.5 = 11.38 = ~11.
	mmio_write(UART0_FBRD, 11);
 
	// Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9)); 

	// Setup atomic lock
	spinInit(uartLock);
}
 
void uart_putc(unsigned char c)
{
	// Wait for UART to become ready to transmit.
	while ( mmio_read(UART0_FR) & (1 << 5) ) { }
	mmio_write(UART0_DR, c);
}
 
unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}
 
void uart_print(const char* str)
{
	spinLock(uartLock);

	for (size_t i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);
	
	uart_putc('\r');
	uart_putc('\n');
	uart_putc('\0');

	spinUnlock(uartLock);
}

void uart_printi(const char* str)
{
	spinLock(uartLock);

	for (size_t i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);

	spinUnlock(uartLock);
}

void uart_dec(unsigned int x)
{
	spinLock(uartLock);

	unsigned int i = x;
	unsigned int dig=1;
 	while (i/=10) dig++;
	int arrDig = dig;
	
	char arr[dig+1];
	while (dig--)
	{
		arr[dig]=x%10 + 48;
		x/=10;
	}
	arr[arrDig] = '\0';
	

	for (size_t i = 0; arr[i] != '\0'; i ++)
		uart_putc((unsigned char)arr[i]);

	spinUnlock(uartLock);
}

void uart_hex(unsigned int x, uint8_t formatting)
{
	spinLock(uartLock);

	if (formatting) uart_putc('0');
	if (formatting) uart_putc('x');

	unsigned int i = x;
	unsigned int dig=1;
 	while (i/=16) dig++;
	int arrDig = dig;
	
	char arr[dig+1];
	while (dig--)
	{
		int value = x%16 + 48;
		
		if (value > 57) // If ASCII is greater than 9 wrap back down the letters
			value += 7;

		arr[dig] = value;
		x/=16;
	}
	arr[arrDig] = '\0';
	

	for (size_t i = 0; arr[i] != '\0'; i ++)
		uart_putc((unsigned char)arr[i]);
	if (formatting) uart_putc('\r');
	if (formatting) uart_putc('\n');
	if (formatting) uart_putc('\0');

	spinUnlock(uartLock);
}