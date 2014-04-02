/* 
 * File:   main.c
 * Author: Matt Staniszewski
 *
 * Created on March 31, 2014, 11:38 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

/*
 * 
 */

// Configuration Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = ECPLL     // Oscillator Selection bits (EC oscillator, PLL enabled and under software control, CLKO function on OSC2)
#pragma config FOSC2 = OFF      // Default/Reset System Clock Select bit (INTRC enabled as system clock when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

// Definitions
#define _XTAL_FREQ 31000
#define LED_GRN 0x01

// Function Prototypes
void blink(unsigned int);

int main(int argc, char** argv) {

    // Bring all Outputs Low
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    // Disable analog I/O on all ports
    ADCON1 = 0x00;

    // Set RA0 as an output
    TRISA |= 0x01;

    while(1)
    {
        blink(LED_GRN);
        __delay_ms(1000);
    }
    
}

// Blink the given LED
void blink(unsigned int LED)
{
   PORTA |= LED;
   __delay_ms(75);
   PORTA &= ~LED;
}

