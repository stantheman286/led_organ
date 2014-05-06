/* CCS C code for RGB LED strips driven by the LPD8806 IC

Note we technically work with 8 bit R/G/B values despite the strip only supporting the lower 7 bits (0-127/0x00-0x7F).
Mostly because I was too lazy to completely re-engineer my ws2801 example code.

These are much, much faster than the ws2801 strips, especially on longer runs. I ran 320 LEDs with a PIC clocked at 40mhz with
no issues.

Inspired by the Adafruit library at https://github.com/adafruit/LPD8806 - they have done a great job reverse engineering
this LED strip!

This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

-Patrick Cantwell (3/30/2012)
*/

#include "led_organ.h"

#define LED_STRIP_LEN 32 // number of LEDs on the strip
uint32_t led_strip_colors[LED_STRIP_LEN];

void write_latch(uint16_t len) {

  // Latch length varies with the number of LEDs:
  len = ((len + 63) / 64) * 3;

  while(len>0) {
    WriteSPI(0x00);
    len--;
  }
}

void send_frame() {
  uint16_t i;
  uint8_t red,green,blue;

  for(i=0;i<LED_STRIP_LEN;i++) {
    blue = led_strip_colors[i] & 0xFF;
    green = (led_strip_colors[i] >> 8) & 0xFF;
    red = (led_strip_colors[i] >> 16) & 0xFF;
    WriteSPI((green|0x80));
    WriteSPI((red|0x80));
    WriteSPI((blue|0x80));
  }
  write_latch(LED_STRIP_LEN);
}

void addRandom(void) {
  uint16_t x;
  uint32_t new_color = 0;

  //First, shuffle all the current colors down one spot on the strip
  for(x = (LED_STRIP_LEN - 1) ; x > 0 ; x--)
    led_strip_colors[x] = led_strip_colors[x - 1];

  //Now form a new RGB color
  for(x = 0 ; x < 3 ; x++){
    new_color <<= 8;
    new_color |= rand(); //Give me a number from 0 to 0xFF
  }
  led_strip_colors[0] = new_color; //Add the new random color to the strip
}

void demoLEDs()
{
    uint16_t i;

    for(i = 0 ; i < 400 ; i++)
        __delay_ms(5);
    for(i = 0 ; i < LED_STRIP_LEN ; i++) {led_strip_colors[i] = 0;}
    send_frame();
    

    led_strip_colors[0] = 0xFF0000;  // red
    led_strip_colors[1] = 0x00FF00;  // green
    led_strip_colors[2] = 0x0000FF;  // blue
    led_strip_colors[3] = 0x00FFFF;  // green + blue
    led_strip_colors[4] = 0xFF00FF;  // red + blue
    led_strip_colors[5] = 0xFFFF00;  // red + green
    send_frame();

    for(i = 0 ; i < 1000 ; i++)
        __delay_ms(5);

    for(i=0;i<LED_STRIP_LEN;i++) {                       // Let's be patriotic! (Red, White, Blue)
      if((i%3) == 0) {led_strip_colors[i] = 0xFF0000;}
      if((i%3) == 1) {led_strip_colors[i] = 0xFFFFFF;}
      if((i%3) == 2) {led_strip_colors[i] = 0x0000FF;}
    }
    send_frame();


    for(i = 0 ; i < 2000 ; i++)
        __delay_ms(5);

    for(i=0;i<LED_STRIP_LEN;i++) {                       // Happy St. Patrick's Day! (Green, White)
      if((i%2) == 0) {led_strip_colors[i] = 0x00FF00;}
      if((i%2) == 1) {led_strip_colors[i] = 0xFFFFFF;}
    }
    send_frame();

    for(i = 0 ; i < 2000 ; i++)
        __delay_ms(5);

    for(i=0;i<LED_STRIP_LEN;i++) {                       // Let's go Yankees! (Blue, White)
      if((i%2) == 0) {led_strip_colors[i] = 0x0000FF;}
      if((i%2) == 1) {led_strip_colors[i] = 0xFFFFFF;}
    }
    send_frame();

    for(i = 0 ; i < 2000 ; i++)
        __delay_ms(5);

    for(i = 0 ; i < LED_STRIP_LEN ; i++) {led_strip_colors[i] = 0;}  // Clear the strip, then...
    send_frame();

    while(1) {                                                    // Stuff random colors down the strip forever.
      addRandom();
      send_frame();
      for(i = 0 ; i < 20 ; i++)
        __delay_ms(5);
    }
  
}