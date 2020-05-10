#include "pages.h"
#include "../io/uart.h"
#include "../stdlib.h"
#include "heap.h"

extern int __end; // Defined in linker.ld as the end of the kernel

static struct page* pageListArray;

static unsigned int memorySize;
static uint32_t pageSize;
static size_t numPages;
static size_t kernelPages;

//void initMemory(uint32_t atagPointer) {}


void initMemory(size_t atagPointer)
{

    // Get memory and page size
    memorySize = getMemorySize((struct atag*)atagPointer);
    pageSize = getPageSize((struct atag*)atagPointer);

    // Work out how many pages we need
    numPages = memorySize / pageSize;
    uart_printi("\r\nInitialising memory with page size ");
    uart_dec(pageSize);
    uart_printi(" bytes - ");
    uart_dec(numPages);
    uart_print(" pages");

    // Allocate enough space for our page data
    size_t pageArrayLength = sizeof(struct page) * numPages;
    pageListArray = (struct page*)&__end + pageSize; // Add page size for MMU?
    bzero(pageListArray, pageArrayLength);

    // Allocate space for the kernel
    kernelPages = (((size_t)&__end) + pageArrayLength) / pageSize;
    int i;
    for (i = 0; i < kernelPages; ++i)
    {
        pageListArray[i].virtualAddress = i * pageSize;
        pageListArray[i].flags.allocated = 1;
        pageListArray[i].flags.kernel = 1;
    }
    
    // Allocate space for the 1MB kernel heap
    for (; i < kernelPages + (KERNEL_HEAP_SIZE / pageSize); ++i)
    {
        pageListArray[i].virtualAddress = i * pageSize;
        pageListArray[i].flags.allocated = 1;
        pageListArray[i].flags.kernel = 1;
    }

    // Init heap
    size_t pageArrayEnd = &pageListArray + pageArrayLength;
    initKernelHeap(pageArrayEnd);

    // Leave the rest of the space free for user space
    for (; i < numPages; ++i)
    {
        pageListArray[i].virtualAddress = 0;
        pageListArray[i].flags.kernel = 0;
        pageListArray[i].flags.allocated = 0;
    }
    uart_print("Memory initialised\r\n");
    
    uart_printi("Total free pages: ");
    uart_dec(getFreePages());
    uart_print("\r\n");

    size_t pageAddr = allocatePage();

    uart_print("");
    freePage(pageAddr);

    uart_print("Testing heap...");

    int* testInt = kmalloc(sizeof(int));
    *testInt = 420;

    uart_printi("Heap allocated successfully at location ");
    uart_hex(testInt, 1);

    kfree(testInt);
    if (*testInt == 0) uart_print("Heap kfree working successfully");
    else uart_print("Heap kfree error!");

}


size_t allocatePage()
{
    uart_print("Allocating page...");

    // Find a free page
    for (size_t i = 0; i < numPages; ++i)
    {
        if (!pageListArray[i].flags.allocated && !pageListArray[i].flags.kernel)
        {
            // Allocate it
            pageListArray[i].flags.allocated = 1;
            pageListArray[i].flags.kernel = 1;

            size_t physicalLocation = &pageListArray[i];
            bzero((size_t*)physicalLocation + sizeof(struct page), pageSize - sizeof(struct page));
            
            uart_printi("Page allocated at address ");
            uart_hex(physicalLocation, 1);

            uart_printi("Total free pages: ");
            uart_dec(getFreePages());
            uart_print("");

            return physicalLocation;
        }
    }
    return 0;
}

void freePage(size_t address)
{
    uart_print("Freeing page...");

    // Get the page
    struct page* page  = (struct page*)address;

    // Mark as free
    page->flags.kernel = 0;
    page->flags.allocated = 0;
    page->virtualAddress = 0;

    uart_printi("Page freed. Total free pages: ");
    uart_dec(getFreePages());
    uart_print("\r\n");
}

size_t getFreePages() // Very inefficient, linked lists are boring and so is just using a variable ;)
{
    size_t freePages = 0;
    for (size_t i = 0; i < numPages; ++i)
    {
        if (!pageListArray[i].flags.allocated && !pageListArray[i].flags.kernel)
            freePages++;
    }

    return freePages;
}