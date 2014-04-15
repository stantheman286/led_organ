#include "led_organ.h"

void setupSPI() {

    // Setup SPI Port (no CS)
    TRISCbits.TRISC5= 0;   // RC5/SDO - Output (Serial Data Out)
    TRISCbits.TRISC4= 1;   // RC4/SDI - Input (Serial Data In)
    TRISCbits.TRISC3= 0;   // RC3/SCK - Output (Clock)

    // Setup Registers
    SSPSTAT = 0x40;        // Set SMP=0 and CKE=1. Notes: The lower 6 bits are read-only
    SSPCON1 = 0x20;        // Enable SPI Master with Fosc/4

}

void writeLEDSPI(uint8_t data)
{

  // Start Data transmission
  SSPBUF = data;

  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);

}