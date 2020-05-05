#include "stdlib.h"

#include <stdint.h>

void memcpy(void * dest, const void * src, int bytes)
{
    char * d = dest;
    const char * s = src;
    while (bytes--) {
        *d++ = *s++;
    }
}

void bzero(void * dest, int bytes)
{
    memset(dest, 0, bytes);
}

void memset(void * dest, uint8_t c, int bytes)
{
    uint8_t * d = dest;
    while (bytes--) {
        *d++ = c;
    }
}