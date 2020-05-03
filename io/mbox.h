#include <stdint.h>

#define ArmToVC 8
#define VcToArm 9

volatile unsigned int mailbox[36];

void createSerialMailbox();
void createSizeMailbox();
uint32_t sendMailbox(uint8_t channel);