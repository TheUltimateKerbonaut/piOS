#include "stdint.h"

#ifndef ATAG_H
#define ATAG_H

typedef enum
{
    NONE = 0x00000000,
    CORE = 0x54410001,
    MEM = 0x54410002
} atag_tag;

struct atag_core
{
    uint32_t flags; // Bit 0 is read-only
    uint32_t pageSize; // System page size is usually 4k
    uint32_t rootdev; // Root device number
};

struct atag_mem
{
    uint32_t size; // Size of the area
    uint32_t start; // Physical start address
};

struct atag
{
    uint32_t tag_size;
    atag_tag tag;
    union
    {
        struct atag_mem mem;
        struct atag_core core;
    };
};

static struct atag_mem* a_mem;
static struct atag_core* a_core;

int populateFromAtag(struct atag* tag);
uint32_t getMemorySize(struct atag* tag);
uint32_t getPageSize(struct atag* tag);

#endif