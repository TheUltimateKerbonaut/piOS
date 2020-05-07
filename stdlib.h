#include <stdint.h>

#ifndef STDLIB_H
#define STDLIB_H

void memcpy(void * dest, const void * src, int bytes);
void bzero(void * dest, int bytes);
void memset(void * dest, uint8_t c, int bytes);

#endif