#include "mbox.h"
#include "uart.h"

/*

    To read from a mailbox:
        Read the status register until the empty flag is not set
        Read data from the read register
        If the lower four bits do not match the channel number desired then repeat from 1
        The upper 28 bits are the returned data

    To write to a mailbox
        Read the status register until the full flag is not set
        Write the data (shifted into the upper 28 bits) combined with the channel (in the lower four bits) to the write register

*/

#define VIDEOCORE_MBOX  (0x3F000000+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

void createSerialMailbox()
{
    mailbox[0] = 8*4;                  // length of the message
    mailbox[1] = 0x0;         // this is a request message
    
    mailbox[2] = 0x10004;   // get serial number command
    mailbox[3] = 8;                    // buffer size
    mailbox[4] = 8;
    mailbox[5] = 0;                    // clear output buffer
    mailbox[6] = 0;

    mailbox[7] = 0x0;
}

void createSizeMailbox()
{
    mailbox[0] = 8*4;                   // length of the message
    mailbox[1] = 0x0;                   // this is a request message
    
    mailbox[2] = 0x00040004;            // replace last number with 3 to get physical resolution
    mailbox[3] = 8;                     // buffer size
    mailbox[4] = 8;
    mailbox[5] = 0;                     // clear output buffer
    mailbox[6] = 0;

    mailbox[7] = 0x0;
}

uint32_t sendMailbox(uint8_t channel)
{
    // Read status register until the full flag is not set
    while (*MBOX_STATUS & MBOX_FULL) { };

    // Write the address of the data and the channel to the mailbox
    uint32_t packet  = ((uint32_t)(uint64_t)&mailbox &~ 0xF) | channel;
    *MBOX_WRITE = packet;

    // Wait for a response
    for (;;)
    {
        // Wait until the status register's empty flag is not set
        while (*MBOX_STATUS & MBOX_EMPTY) { };

        // Read data
        uint32_t recievedData = *MBOX_READ;
        uint8_t recievedChannel = recievedData & 0xF;

        // If the two are identical and the message has come from the GPU, it's the right response
        if (recievedData == packet && recievedChannel == channel && mailbox[1] == MBOX_RESPONSE) return recievedData;
        else if (recievedData == packet && recievedChannel == channel) return 0;
    }
}