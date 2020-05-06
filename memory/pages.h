#ifndef PAGES_H
#define PAGES_H

#include "../io/atag.h"
#include <stdint.h>

struct pageFlag
{
    uint8_t allocated: 1;
    uint8_t kernel: 1;
    uint8_t padding: 6;
};

struct page
{
    uint32_t virtualAddress;
    struct pageFlag flags;
};

void initMemory(uint32_t atagPointer);

uint32_t allocatePage(void);

#endif