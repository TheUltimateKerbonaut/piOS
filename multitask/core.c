#include "core.h"
#include "../io/uart.h"
#include "lock.h"

int bob = 10;

void initCore()
{
    uart_dec(attemptLock(&stdoutLock));

    for (int i = 0; i < 1000000000; ++i)
    {
        bob += 1;
    }
    uart_print("Hello world");
}