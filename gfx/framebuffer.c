#include "framebuffer.h"
#include "../io/uart.h"
#include "../mbox.h"

#define WIDTH 640
#define HEIGHT 480
#define DEPTH 16

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

    if (sendMailbox(ArmToVC) != 0 && mailbox[20] == DEPTH && mailbox[28] !=0) // If depth is 32 and the pointer is set
    {
        mailbox[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
        resolutionX = mailbox[5];   
        resolutionY = mailbox[6];   
        fbPitch = mailbox[33];
        fbAddress = (void*)((uint64_t)mailbox[28]);
        uart_printi("Successfully constructed framebuffer with resolution ");
        uart_dec(WIDTH);
        uart_printi("x");
        uart_dec(HEIGHT);
        uart_printi(" with depth ");
        uart_dec(DEPTH);
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

    for (int x = 0; x < resolutionX; ++x)
        for (int y = 0; y < resolutionY; ++y)
        fbSetPixel(x, y, 0x11111111);

    return 0;

}

void fbSetPixel(int x, int y, uint32_t colour)
{
    uint32_t offset = (y * fbPitch) + (x * 4);
    volatile uint32_t* framePtr = (uint32_t*)(fbAddress + offset);
    *framePtr = colour;
}