#include <stdint.h>

int resolutionX;
int resolutionY;
int colourDepth;

uint32_t fbAddress;
uint32_t fbSize;
uint32_t fbPitch;

int initFramebuffer();
void fbSetPixel(int x, int y, uint32_t colour);