#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

struct heapSegment
{
    struct heapSegment* next;
    struct heapSegment* previous;
    uint8_t isAllocated;
    size_t segmentSize;
};

void* kmalloc(size_t bytes);
void kfree(void* pointer);

static size_t KERNEL_HEAP_SIZE = 0x1000000;

void initKernelHeap(size_t heapStart);

#endif