#include "core.h"
#include "../gfx/framebuffer.h"
#include "lock.h"

void initCore()
{
    fbWriteString("Hello from core 1!");
}