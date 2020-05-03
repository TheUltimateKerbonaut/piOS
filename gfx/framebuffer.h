#include <stdint.h>

int fbResolutionX;
int fbResolutionY;
int fbColourDepth;

uint32_t fbAddress;
uint32_t fbSize;
uint32_t fbPitch;

uint32_t fbTextRows;
uint32_t fbTextColumns;

uint32_t fbTextX;
uint32_t fbTextY;

int initFramebuffer();
void fbSetPixel(int x, int y, uint32_t colour);
void fbWriteChar(char character);
void fbWriteString(char* string);

static inline uint32_t getColour(int r, int g, int b) // 16 bit - max 0xFFFF
{
    return (r << 16 | g << 8 | b);
}   