#include "framebuffer.h"
#include "../io/uart.h"
#include "../mbox.h"
#include "charset.h"

#define WIDTH 640
#define HEIGHT 480
#define DEPTH 32

static int fbResolutionX;
static int fbResolutionY;
static int fbColourDepth;

static size_t fbAddress;
static uint32_t fbSize;
static uint32_t fbPitch;

static uint32_t fbTextRows;
static uint32_t fbTextColumns;

static uint32_t fbTextX;
static uint32_t fbTextY;

int initFramebuffer()
{
    mailbox[0] = 35*4;
    mailbox[1] = 0;

    mailbox[2] = 0x48003;  // Set physical dimensions
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = WIDTH;
    mailbox[6] = HEIGHT;

    mailbox[7] = 0x48004; // Set virtual dimensions
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = WIDTH;
    mailbox[11] = HEIGHT;

    mailbox[12] = 0x48009; // Set virtual offset
    mailbox[13] = 8;
    mailbox[14] = 8;
    mailbox[15] = 0;
    mailbox[16] = 0;

    mailbox[17] = 0x48005; // Set depth
    mailbox[18] = 4;
    mailbox[19] = 4;
    mailbox[20] = DEPTH;

    mailbox[21] = 0x48006; // Set pixel order
    mailbox[22] = 4;
    mailbox[23] = 4;
    mailbox[24] = 1; // RGB

    mailbox[25] = 0x40001; // Allocate space and get framebuffer address
    mailbox[26] = 8;
    mailbox[27] = 8;
    mailbox[28] = 4096; // Pointer
    mailbox[29] = 0; // Size

    mailbox[30] = 0x40008; // Get pitch
    mailbox[31] = 4;
    mailbox[32] = 4;
    mailbox[33] = 0;

    mailbox[34] = 0;

    if (sendMailbox(ArmToVC) != 0 && mailbox[20] == DEPTH && mailbox[28] !=0) // If depth is right and the pointer is set
    {
        mailbox[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
        fbResolutionX = mailbox[5];   
        fbResolutionY = mailbox[6];   
        fbPitch = mailbox[33];
        fbAddress = (size_t)((uint32_t)mailbox[28]);
        fbColourDepth = mailbox[20];
        uart_printi("Successfully constructed framebuffer with resolution ");
        uart_dec(fbResolutionX);
        uart_printi("x");
        uart_dec(fbResolutionY);
        uart_printi(" with depth ");
        uart_dec(fbColourDepth);
        uart_print("");
    } else 
    {
        uart_printi("Unable to construct framebuffer with resolution ");
        uart_dec(WIDTH);
        uart_printi("x");
        uart_dec(HEIGHT);
        uart_printi(" with depth ");
        uart_dec(DEPTH);
        uart_print("");
        return 1;
    }

    for (int x = 0; x < fbResolutionX; ++x)
        for (int y = 0; y < fbResolutionY; ++y)
        fbSetPixel(x, y, getColour(0, 0, 0)); // 0x11111111

    fbTextRows = fbResolutionY / CHAR_HEIGHT;
    fbTextColumns = fbResolutionX / CHAR_WIDTH;
    fbTextX = 0;
    fbTextY = 0;

    fbWriteString("Hello world!");

    return 0;

}

void fbSetPixel(int x, int y, uint32_t colour)
{
    uint32_t offset = (y * fbPitch) + (x * 4);
    volatile uint32_t* framePtr = (uint32_t*)(fbAddress + offset);
    *framePtr = colour;
}

void fbWriteChar(char character)
{
    const uint32_t WHITE = getColour(255, 255, 255);
    const uint32_t BLACK = getColour(0, 0, 0);

    const uint8_t* bitmap = font(character);

    if (character == '\n')
    {
        fbTextX = 0;
        fbTextY++;
        return;
    }

    for (int w = 0; w < CHAR_WIDTH; ++w)
    {
        for (int h = 0; h < CHAR_HEIGHT; ++h)
        {
            uint8_t mask = 1 << (w);
            if (bitmap[h] & mask)
                fbSetPixel(fbTextX * CHAR_WIDTH + w, fbTextY * CHAR_HEIGHT + h, WHITE);
            else
                fbSetPixel(fbTextX * CHAR_WIDTH + w, fbTextY * CHAR_HEIGHT + h, BLACK);
        }
    }

    fbTextX++;

    if (fbTextX >= fbTextColumns) 
    {
        fbTextX = 0;
        fbTextY++;   
    }
}

void fbWriteString(char* string)
{
    for (size_t i = 0; string[i] != '\0'; i ++)
		fbWriteChar((unsigned char)string[i]);
	fbWriteChar('\n');
}
