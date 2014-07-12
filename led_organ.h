/* 
 * File:   led_organ.h
 * Author: matt
 *
 * Created on April 7, 2014, 7:32 PM
 */

#ifndef LED_ORGAN_H
#define	LED_ORGAN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include <spi.h>
#include <htc.h>

// Configuration Settings
#define USE_AND_OR

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = EC        // Oscillator Selection bits (EC oscillator, CLKO function on OSC2)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT    // CCP2 MUX bit (CCP2 is multiplexed with RC1)

// Definitions
#define _XTAL_FREQ 20000000ULL // 20 MHz
#define LED_RED 0x20
#define LED_GRN 0x01
#define LED_BLU 0x01
#define RAND_MAX 255

// Common useful definitions
#define ON 	1
#define OFF	0
#define	WRITE	0
#define READ	1
#define FALSE	0
#define TRUE	1

// Peak indicator LEDs
#define HIGHPEAK	PORTBbits.RB5
#define MIDPEAK		PORTBbits.RB4
#define LOWPEAK		PORTBbits.RB3

// LED string outputs
#define HIGHSTRING	PORTBbits.RB2
#define MIDSTRING	PORTBbits.RB1
#define LOWSTRING	PORTBbits.RB0

// Toggle Switch pins
#define SWITCHLOW   0 //MS: manually assigned for now instead of pins
#define SWITCHHIGH  0

// ADC defintions
#define HIGHADC		0
#define MIDADC		1
#define LOWADC		2
#define MAINADC		3

// LED PWM steps
#define PWMSTEPS	31

// AGC Settings
// MIN and MAX is the range of the digital pot (0-255)
// TARGETGAIN is the average gain level measured by the
// ADC (0-1023)

// I set these parameters by monitoring the average
// levels read by the PIC (by connecting an LCD screen to
// the TOMTE.  Your milage may vary since the audio component
// tolerances will differ from circuit to circut.  If the lights
// don't seem to be firing enough try raising the TARGETGAIN.
#define MINGAIN		10		// Range 0-254
#define MAXGAIN		250		// Range 0-254
#define TARGETGAIN	100		// Range 0-1023

// Peak levels
// These set the peak value for each of the channels. Alter these to 
// make a tone level light up more or less (for a given stable gain level)
// Range is 0-1023
#define LOWPEAK_VALUE	470
#define MIDPEAK_VALUE	520
#define HIGHPEAK_VALUE	150

// Sound watermark level
// This is the input gain at which TOMTE switches to colour organ mode
// when waiting for music to play (0-1024)
#define WATERMARK	400

// External Function Prototypes
extern void demoLEDs();
extern void setupSPI();
extern void writeLEDSPI(uint8_t);
extern void setGainLevel(unsigned char);
extern int readMyADC(unsigned char);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_ORGAN_H */

