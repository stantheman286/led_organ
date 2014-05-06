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

// External Function Prototypes
extern void demoLEDs();
extern void setupSPI();
extern void writeLEDSPI(uint8_t);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_ORGAN_H */

