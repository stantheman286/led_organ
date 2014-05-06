/* 
 * File:   main.c
 * Author: Matt Staniszewski
 *
 * Created on March 31, 2014, 11:38 PM
 */

#include "led_organ.h"

// Function Prototypes
void blink_b(unsigned int);
void blink_c(unsigned int);

int main(int argc, char** argv) {

    // Bring all Outputs Low
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    // Disable analog I/O on all ports
    ADCON1 = 0x0F;

    // Set RB0, RC0 as outputs
    TRISB = 0x00;
//    TRISC = 0x00;

    // Setup SPI Interface
    setupSPI();

    while(1)
    {
        // Turn on LED
//        PORTB |= LED_RED;

        // Demo LED Bar
        demoLEDs();
               
        /*
        blink_b(LED_RED);
        for(int i = 0 ; i < 100 ; i++)
            __delay_ms(5);
        blink_b(LED_GRN);
        __delay_ms(150);
        blink_c(LED_BLU);
        __delay_ms(150);
         */
    }
            
}

// Blink the given LED on Port B
void blink_b(unsigned int LED)
{
   PORTB |= LED;
   for(int i = 0 ; i < 100 ; i++)
    __delay_ms(5);
   PORTB &= ~LED;
}

// Blink the given LED on Port C
void blink_c(unsigned int LED)
{
   PORTC |= LED;
   for(int i = 0 ; i < 100 ; i++)
    __delay_ms(5);
   PORTC &= ~LED;
}

