#include "pages.h"
#include "../io/uart.h"
#include "../stdlib.h"

extern int __end; // Defined in linker.ld as the end of the kernel

static struct page* pageListArray;

void initMemory(uint32_t atagPointer)
{
    // Get memory and page size
    uint32_t memorySize = getMemorySize((struct atag*)atagPointer);
    uint32_t pageSize = getPageSize((struct atag*)atagPointer);

    // Work out how many pages we need
    uint32_t numPages = memorySize / pageSize;
    uart_printi("Initialising memory with page size ");
    uart_dec(pageSize);
    uart_printi(" bytes - ");
    uart_dec(numPages);
    uart_print(" pages");

    // Allocate enough space for our page data
    uint32_t pageArrayLength = sizeof(struct page) * numPages;
    pageListArray = (struct page*)&__end;
    //bzero(&pageList, pageArrayLength);

    // Allocate space for the kernel
    uint32_t kernelPages = (((uint32_t)&__end) + pageArrayLength) / pageSize;
    int i;
    for (i = 0; i < kernelPages; ++i)
    {
        pageListArray[i].virtualAddress = i * pageSize;
        pageListArray[i].flags.allocated = 1;
        pageListArray[i].flags.kernel = 1;
    }

    // Leave the rest of the space free for user space
    for (; i < numPages; ++i)
    {
        pageListArray[i].virtualAddress = 0;
        pageListArray[i].flags.kernel = 0;
        pageListArray[i].flags.allocated = 0;
    }

    uart_print("done");

}