#include "core.h"
#include "../gfx/framebuffer.h"
#include "lock.h"


void initCore()
{
    fbWriteString("Hello from core 1!");

    while (1)
    {
        for (int i = 0; i < 0xFFFFFF; ++i) asm volatile("nop");
        fbWriteString("Core 1 says hi!");
    }
}