#ifndef CORE_H
#define CORE_H

void initCore();
static inline void sendCoreEvent()
{
	asm volatile("sev\n");
}

extern void setupStack();

extern int bob;

#endif