#ifndef CORE_H
#define CORE_H

#include "lock.h"

void initCore();
static inline void sendCoreEvent()
{
	asm volatile("sev\n");
}

extern void setupStack();

#endif