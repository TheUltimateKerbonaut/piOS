#include <stdint.h>

#ifndef MBOX_H
#define MBOX_H

#define ArmToVC 8
#define VcToArm 9

extern volatile unsigned int mailbox[36];

void createSerialMailbox();
void createSizeMailbox();
uint32_t sendMailbox(uint8_t channel);

#endif