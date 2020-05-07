#include "exceptionLevel.h"

#include "uart.h"

void getExceptionState()
{
    uart_printi("Current exception level is ");
    uart_dec(getExceptionLevel());
    uart_print("\r\n");
}