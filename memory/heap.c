#include "heap.h"
#include "../io/uart.h"
#include "../stdlib.h"

#include <limits.h>

struct heapSegment* heapSegmentListHead;

/* 
    To allocate a segment of the heap, we will look for the allocation that best
    fits the number of requested bytes and is not in sue. If the allocation is
    relatively large in size, we will split it into two and only use one. In
    this implementation, an allocation needs to be split if it is at least
    twice the size of a header.
*/

void* kmalloc(size_t bytes)
{
    struct heapSegment* currentSegment = NULL;
    struct heapSegment* bestSegment = NULL;

    int bytesDifference = 0;
    int bestDifference = INT_MAX; // Max value for signed int

    // Allocate space for the header within the allocation and make
    // sure it is 16-byte aligned
    bytes += sizeof(struct heapSegment);
    bytes += bytes % 16 ? 16 - (bytes % 16) : 0;
    
    // Find the allocation that is closest in size to the request
    for (currentSegment = heapSegmentListHead; currentSegment != NULL; currentSegment = currentSegment->next)
    {
        // Work out difference of bytes
        bytesDifference = currentSegment->segmentSize - bytes;

        // If it's the best difference yet, and it's not allcoated, take note
        if (!currentSegment->isAllocated && bytesDifference < bestDifference && bytesDifference >= 0)
        {
            bestSegment = currentSegment;
            bestDifference = bytesDifference;
        }
    }

    // If there's no free memory, panic
    if (bestSegment == NULL) return NULL;

    // Apply splitting logic if nessecary
    if (bestDifference > (int)(2 * sizeof(struct heapSegment)))
    {
        bzero(((void*)(bestSegment)) + bytes, sizeof(struct heapSegment));
        currentSegment = bestSegment->next;
        bestSegment->next = ((void*)(bestSegment)) + bytes;
        bestSegment->next->next = currentSegment;
        bestSegment->next->previous = bestSegment;
        bestSegment->next->segmentSize = bestSegment->segmentSize - bytes;
        bestSegment->segmentSize = bytes;
    }
    

    bestSegment->isAllocated = 1;

    return bestSegment + 1; // Add 1 to get memory directly after the header
}

void kfree(void* pointer)
{
    struct heapSegment* segment = (struct heapSegment*) (pointer - sizeof(struct heapSegment));
    segment->isAllocated = 0;

    // Bzero it out
    bzero(segment, segment->segmentSize);

    // Try to coalesce segements to the left
    while(segment->previous != NULL && !segment->previous->isAllocated)
    {
        segment->previous->next = segment->next;
        segment->previous->segmentSize += segment->segmentSize;
        segment = segment->previous;
    }

    // Try to coalesce segments to the right
    while(segment->next != NULL && !segment->next->isAllocated) 
    {
        segment->segmentSize += segment->next->segmentSize;
        segment->next = segment->next->next;
    }
}

void initKernelHeap(size_t heapStart)
{
    uart_print("Initialising heap...");
    heapSegmentListHead = (struct heapSegment*)heapStart;
    bzero(heapSegmentListHead, sizeof(struct heapSegment));
    heapSegmentListHead->segmentSize = KERNEL_HEAP_SIZE;
}