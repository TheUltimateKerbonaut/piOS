#include <stdint.h>
#include <stddef.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

int initFramebuffer();
void fbSetPixel(int x, int y, uint32_t colour);
void fbWriteChar(char character);
void fbWriteString(char* string);

static inline uint32_t getColour(int r, int g, int b) // 16 bit - max 0xFFFF
{
    return (r << 16 | g << 8 | b);
}   

#endif