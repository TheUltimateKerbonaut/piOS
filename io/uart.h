#include <stdint.h>
#include <stddef.h>

#include "../multitask/lock.h"

void uart_init(uint8_t raspi);
void uart_putc(unsigned char c);
unsigned char uart_getc();
void uart_print(const char* str);
void uart_printi(const char* str);
void uart_dec(unsigned int x);
void uart_hex(unsigned int x, uint8_t formatting);

static lock stdoutLock;