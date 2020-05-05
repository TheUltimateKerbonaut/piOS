#include "atag.h"

int populateFromAtag(struct atag* tag)
{
    if (tag->tag_size != 5) return tag->tag_size; // 2 if no data, 5 if data

    while (tag->tag != NONE)
    {
        if (tag->tag == MEM) a_mem = &(tag->mem);
        if (tag->tag == CORE) a_core = &(tag->core);

        tag = (struct atag*)(((uint32_t*)tag) + tag->tag_size);
    }

    return 0;
}

uint32_t getMemorySize(struct atag* tag)
{
    return a_mem->size;
}

uint32_t getPageSize(struct atag* tag)
{
    //return a_core->pageSize;
    return (a_core->pageSize == 0) ? 4096 : a_core->pageSize;
}