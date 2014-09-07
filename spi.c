#include "led_organ.h"

void setupSPI() {

    // Setup LED SPI Port (no CS)
    TRISCbits.TRISC5 = 0;   // RC5/SDO - Output (Serial Data Out)
    TRISCbits.TRISC4 = 1;   // RC4/SDI - Input (Serial Data In)
    TRISCbits.TRISC3 = 0;   // RC3/SCK - Output (Clock)

    // Setup and disable CS for MP41010
    TRISCbits.TRISC0 = 0;
    PORTCbits.RC0 = 1;

    // Setup Registers
    SSPSTAT = 0x40;        // Set SMP=0 and CKE=1. Notes: The lower 6 bits are read-only
    SSPCON1 = 0x20;        // Enable SPI Master with Fosc/4

}

// Set the level of the MP41010 digital pot controlling the input gain
void setGainLevel(unsigned char gainLevel)
{
	// Set up SPI for the MP41010
	SSPSTAT = 0xC0;	// SPI Master
                  // Tx occurs on idle to active transition

	SSPCON1 = 0x20; // Serial port enabled
							    // Speed is Fosc/4

	// Enable chip serial
	PORTCbits.RC0 = 0;

	// Send SET command
	SSPIF = 0;
	SSPBUF = 0x13;
	while(!SSPIF); // Wait for transmit to complete

	// Send gain level
	SSPIF = 0;
	SSPBUF = gainLevel;
	while(!SSPIF); // Wait for transmit to complete
        
	// Disable chip serial
	PORTCbits.RC0 = 1;
}

