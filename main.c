/************************************************************************
	main.c

    LED String Colour Organ
    Copyright (C) 2009 Simon Inns

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Email: simon.inns@gmail.com

************************************************************************/

//
// Codename: Tone Operated Microprocessor Tree Enhancer (T.O.M.T.E.)
//

// Local includes
#include "led_organ.h"

//// PIC 18F2550 fuse configuration:
//// Config word 1 (Oscillator configuration)
//__CONFIG(1, USBPLL & IESODIS & FCMDIS & HSPLL & CPUDIV1 & PLLDIV5);
//// Config word 2
//__CONFIG(2, VREGEN & PWRTDIS & BORDIS & WDTDIS);
//// Config word 3
//__CONFIG(3, PBDIGITAL & LPT1DIS & MCLREN);
//// Config word 4
//__CONFIG(4, XINSTDIS & STVRDIS & LVPDIS & ICPORTDIS & DEBUGDIS);
//// Config word 5, 6 and 7 (protection configuration)
//__CONFIG(5, UNPROTECT);
//__CONFIG(6, UNPROTECT);
//__CONFIG(7, UNPROTECT);

void main(void)
{

  // Configure on-board ADC
  // Enable AN3-0, Vss and Vdd as voltage references
  ADCON1 = 0x0B;
  
  // Enable ADC, but don't start conversion
  // Start with AN0, just to set something
  ADCON0 = 0x01;

  // Configure the ADC acquisition time according to the datasheet
  ADCON2 = 0xB5; // Note: output is right justified
  
  // Configure ports as inputs (1) or outputs(0)
  TRISA = 0x0F; // Set AN0-3 set as inputs
  TRISB = 0x00; // RB5, LED output
  TRISC = 0x00;
  
  //MS: TODO, add my other code

//	TRISC = 0b00110000; //MS: Was for SPDT for select

	// Clear all ports
	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;

	// Disable the USB device
//	UCON = 0b00000000;  //MS: not implemented on my PIC
//	UCFG = 0b00001000;
    
  // Setup SPI Interface
  setupSPI();

	// Variables for raw volume levels
	int highLevel = 0;
	int midLevel = 0;
	int lowLevel = 0;
	int mainLevel = 0;

	// Set up timer 0
	//
	// Timer 0 provides a 'tick counter' which is used to time the 
	// light transition when TOMTE is not running in colour organ
	// mode.  It increments the tick counter 100 times a second.
	//
  // Fosc is 20,000,000 so Fosc/4 is 5,000,000 ticks/sec
	// meaning 10,000 microseconds is 10,000 * 5 = 50,000 ticks
	// so with a 1:32 prescaler we need to flag every 1563 ticks
	// for 1/10 of a second which means TMR0L should be
	// 65535 - 1563 = 63972 (0xF9E4).
	TMR0IF = 0;			// Clear the timer0 interrupt flag
	TMR0H = 0xF9;
	TMR0L = 0xE4;		// Reset the timer0 counter

	T0CON = 0x84; // Timer0 on, 16-bit and 1:32 prescaler

	// Set the initial input gain level
	int currentGainLevel = 80; // 0-255

        setGainLevel(currentGainLevel);

	// Variables for Automatic Gain Control (AGC)
	int sampleCount = 0;
	long total = 0;
	int average = 0;

	// PWM Variables
	int PWMcount = 0;
	int lowPWM = 0;
	int midPWM = 0;
	int highPWM = 0;
	int fadeCounter = 0;

	// Toggle switch mode
	unsigned char mode = 0;

	// Tick counter
	int tickCounter = 0;

	// Variables for light display only mode
	unsigned char patternNumber = 0;
	unsigned char sequenceNumber = 0;
	int stepNumber = 0;
	int currentStep = 0;
	unsigned char brightness = 0;
	int sequenceRepeats = 0;

	// Variables for sound detection
	int soundDetectedCount = 0;
	unsigned char soundFlag = FALSE;
	int clapCounter = 0;

	while(1)
	{

            
    // MS: TODO Demo LED Bar
    //    demoLEDs();

		// Update the tick counter
		if (TMR0IF == 1)
		{
			tickCounter++;
			if (tickCounter > 1000) tickCounter = 0;

			// Reset timer 0
	    TMR0H = 0xF9;
	    TMR0L = 0xE4;		// Reset the timer0 counter
			TMR0IF = 0;			// Clear the timer0 interrupt flag
		}

		// Read the volume levels
		highLevel = readMyADC(HIGHADC);
		midLevel  = readMyADC(MIDADC);
		lowLevel = readMyADC(LOWADC);
		mainLevel = readMyADC(MAINADC);

		// Read the mode toggle switch
		if (SWITCHLOW == 1 && SWITCHHIGH == 0)
		{
			if (mode != 0)
			{
				mode = 0; // Down

				// Reset all lights to off
				LOWPEAK = 0;
				MIDPEAK = 0;
				HIGHPEAK = 0;
				LOWSTRING = 0;
				MIDSTRING = 0;
				HIGHSTRING = 0;

				// Reset the light display variables
				patternNumber = 0;
				sequenceNumber = 0;
				stepNumber = 0;
				currentStep = 0;
				brightness = 0;
				sequenceRepeats = 0;
			}
		}

		if (SWITCHLOW == 0 && SWITCHHIGH == 0)
		{
			if (mode != 1)
			{
				mode = 1; // Middle
	
				// Reset all lights to off
				LOWPEAK = 0;
				MIDPEAK = 0;
				HIGHPEAK = 0;
				LOWSTRING = 0;
				MIDSTRING = 0;
				HIGHSTRING = 0;
			}
		}

		if (SWITCHLOW == 0 && SWITCHHIGH == 1)
		{
			if (mode != 2)
			{
				mode = 2; // Up
	
				// Reset all lights to off
				LOWPEAK = 0;
				MIDPEAK = 0;
				HIGHPEAK = 0;
				LOWSTRING = 0;
				MIDSTRING = 0;
				HIGHSTRING = 0;
			}
		}

		// Automatic gain control
		total += mainLevel;
		sampleCount++;

		if (sampleCount == 200)
		{
			average = total / sampleCount;

			// Gain limiter
			if (currentGainLevel <= MAXGAIN && currentGainLevel >= MINGAIN)
			{
				if (average < TARGETGAIN) currentGainLevel += 4;
				if (average > TARGETGAIN) currentGainLevel -= 4;
			}
			else
			{
				if (currentGainLevel < MINGAIN) currentGainLevel = MINGAIN + 1;
				if (currentGainLevel > MAXGAIN) currentGainLevel = MAXGAIN - 1;
			}

			setGainLevel(currentGainLevel);

			sampleCount = 0;
			total = 0;
		}

		// Mode 1 (like mode 0 but automatically switches to mode 2 when
		// sound is detected)
		if (mode == 1)
		{
			if (soundFlag == FALSE)
			{
				if (average >= 80) soundDetectedCount++; else soundDetectedCount = 0;
				if (soundDetectedCount > 1500)
				{
					soundFlag = TRUE;
					soundDetectedCount = 0;
				}
			}
			else
			{
				if (average < 50) soundDetectedCount++; else soundDetectedCount = 0;
				if (soundDetectedCount > 10000)
				{
					soundFlag = FALSE;
					soundDetectedCount = 0;
				}
			}

			if (soundFlag == FALSE) LOWPEAK = 1; else LOWPEAK = 0;


			if (soundFlag == TRUE) mode = 2; else mode = 0;
		}

		// Mode 0 (Flashing light display only)
		if (mode == 0)
		{
			// Clap detection (should be fairly insensitive to loud music)
			if ((highLevel > 600) && (clapCounter == 0))
			{
				clapCounter = 1;
			}

			if (clapCounter > 0) clapCounter++;
			if (clapCounter > 1000)
			{
				if (highLevel < 600)
				{
					// Clap detected, go to the next pattern
					clapCounter = 0;
					patternNumber++;
					sequenceNumber = 0;
					stepNumber = 0;
					currentStep = 0;
					brightness = 0;
					sequenceRepeats = 0;
					if (patternNumber > 4) patternNumber = 0;
				}
				else clapCounter = 1;
			}

			if (clapCounter > 0) HIGHPEAK = 1; else HIGHPEAK = 0;

			// Pattern 0: Slow fade in and out of lights in sequence
			if (patternNumber == 0)
			{
				if (sequenceNumber == 0)
				{
					if (currentStep != stepNumber) 
					{
						currentStep = stepNumber;
						if (currentStep < 16) brightness += 2; else brightness -= 2;
					}
					highPWM = brightness;
				} else highPWM = 0;
		
				if (sequenceNumber == 1)
				{
					if (currentStep != stepNumber) 
					{
						currentStep = stepNumber;

						if (currentStep < 16) brightness += 2; else brightness -= 2;
					}
					midPWM = brightness;
				} else if (sequenceNumber != 3)  midPWM = 0;
		
				if (sequenceNumber == 2)
				{
					if (currentStep != stepNumber) 
					{
						currentStep = stepNumber;
						if (currentStep < 16) brightness += 2; else brightness -= 2;
					}
					lowPWM = brightness;
				} else lowPWM = 0;

				if (sequenceNumber == 3)
				{
					if (currentStep != stepNumber) 
					{
						currentStep = stepNumber;

						if (currentStep < 16) brightness += 2; else brightness -= 2;
					}
					midPWM = brightness;
				} else if (sequenceNumber != 1) midPWM = 0;

				if (tickCounter > 6)
				{
					stepNumber++;
					tickCounter = 0;

					if (stepNumber == 31)
					{
						stepNumber = 0;
						sequenceNumber++;
						brightness = 0;
		
						if (sequenceNumber == 4)
						{
							sequenceNumber = 0;
							sequenceRepeats++;
							if (sequenceRepeats > 10)
							{
								sequenceRepeats = 0;
								patternNumber++;
							}
						}
					}
				}
			}

			// Pattern 1: All on
			if (patternNumber == 1)
			{
				highPWM = 31;
				midPWM = 31;
				lowPWM = 31;

				if (tickCounter > 8)
				{
					stepNumber++;
					tickCounter = 0;

					if (stepNumber == 3)
					{
						stepNumber = 0;
						sequenceNumber++;
						brightness = 0;
		
						if (sequenceNumber == 5)
						{
							sequenceNumber = 0;
							sequenceRepeats++;
							if (sequenceRepeats > 40)
							{
								sequenceRepeats = 0;
								patternNumber++;
							}
						}
					}
				}
			}

			// Pattern 2: Random flash
			if (patternNumber == 2)
			{
				if (sequenceNumber == 0)
				{
					if (stepNumber == 0) highPWM = 31; else highPWM = 0;
					if (stepNumber == 1) midPWM = 31; else midPWM = 0;
					if (stepNumber == 2) lowPWM = 31; else lowPWM = 0;
				}
		
				if (sequenceNumber == 1)
				{
					if (stepNumber == 0) highPWM = 31; else highPWM = 0;
					if (stepNumber == 1) lowPWM = 31; else lowPWM = 0;
					if (stepNumber == 2) midPWM = 31; else midPWM = 0;
				}

				if (sequenceNumber == 2)
				{
					if (stepNumber == 0) lowPWM = 31; else lowPWM = 0;
					if (stepNumber == 1) highPWM = 31; else highPWM = 0;
					if (stepNumber == 2) midPWM = 31; else midPWM = 0;
				}
		
				if (sequenceNumber == 3)
				{
					if (stepNumber == 0) lowPWM = 31; else highPWM = 0;
					if (stepNumber == 1) midPWM = 31; else lowPWM = 0;
					if (stepNumber == 2) highPWM = 31; else midPWM = 0;
				}

				if (sequenceNumber == 4)
				{
					if (stepNumber == 0) midPWM = 31; else midPWM = 0;
					if (stepNumber == 1) highPWM = 31; else highPWM = 0;
					if (stepNumber == 2) lowPWM = 31; else lowPWM = 0;
				}

				if (tickCounter > 8)
				{
					stepNumber++;
					tickCounter = 0;

					if (stepNumber == 3)
					{
						stepNumber = 0;
						sequenceNumber++;
						brightness = 0;
		
						if (sequenceNumber == 5)
						{
							sequenceNumber = 0;
							sequenceRepeats++;
							if (sequenceRepeats > 30)
							{
								sequenceRepeats = 0;
								patternNumber++;
							}
						}
					}
				}
			}

			// Pattern 3: Slow fade in and out of all lights at once
			if (patternNumber == 3)
			{
				if (sequenceNumber == 0)
				{
					if (currentStep != stepNumber) 
					{
						currentStep = stepNumber;
						if (currentStep < 16) brightness += 2; else brightness -= 2;
					}
					highPWM = brightness;
					midPWM = brightness;
					lowPWM = brightness;
				}

				if (tickCounter > 3)
				{
					stepNumber++;
					tickCounter = 0;

					if (stepNumber == 31)
					{
						stepNumber = 0;
						sequenceNumber++;
						brightness = 0;
		
						if (sequenceNumber == 1)
						{
							sequenceNumber = 0;
							sequenceRepeats++;
							if (sequenceRepeats > 60)
							{
								sequenceRepeats = 0;
								patternNumber++;
							}
						}
					}
				}
			}

			// Pattern 4: Sequence flash
			if (patternNumber == 4)
			{
				if (sequenceNumber == 0)
				{
					if (stepNumber == 0) highPWM = 31; else highPWM = 0;
					if (stepNumber == 1) midPWM = 31; else if (stepNumber != 3) midPWM = 0;
					if (stepNumber == 2) lowPWM = 31; else lowPWM = 0;
					if (stepNumber == 3) midPWM = 31; else if (stepNumber != 1) midPWM = 0;
				}

				if (tickCounter > 8)
				{
					stepNumber++;
					tickCounter = 0;

					if (stepNumber == 4)
					{
						stepNumber = 0;
						sequenceNumber++;
						brightness = 0;
		
						if (sequenceNumber == 1)
						{
							sequenceNumber = 0;
							sequenceRepeats++;
							if (sequenceRepeats > 100)
							{
								sequenceRepeats = 0;
								patternNumber++;
							}
						}
					}
				}
			}

			// Out of patterns?
			if (patternNumber > 4) patternNumber = 0;

			// PWM output to LED strings
			if (highPWM > PWMcount) HIGHSTRING = 1; else HIGHSTRING = 0;
			if (midPWM > PWMcount) MIDSTRING = 1; else MIDSTRING = 0;		
			if (lowPWM > PWMcount) LOWSTRING = 1; else LOWSTRING = 0;

			// PWMcount controls the range of the PWM
			PWMcount++;
			if (PWMcount > PWMSTEPS) PWMcount = 0;
		}

		// Mode 2 (Colour Organ with PWM - always on)
		if (mode == 2)
		{
			// Fade the PWM levels
			if (fadeCounter == 0)
			{
				if (highPWM > 0) highPWM--;
				if (midPWM > 0) midPWM--;
				if (lowPWM > 0) lowPWM--;
			}
	
			// Output peaks to LED string PWM
			if (highLevel > HIGHPEAK_VALUE) highPWM = PWMSTEPS;
			if (midLevel > MIDPEAK_VALUE)  midPWM = PWMSTEPS;
			if (lowLevel > LOWPEAK_VALUE)  lowPWM = PWMSTEPS;
	
			// PWM output to LED strings
			if (highPWM > PWMcount) HIGHSTRING = 1; else HIGHSTRING = 0;
			if (midPWM > PWMcount) MIDSTRING = 1; else MIDSTRING = 0;		
			if (lowPWM > PWMcount) LOWSTRING = 1; else LOWSTRING = 0;
	
			// Output peaks to peak LEDs
			if (highPWM > PWMcount) HIGHPEAK = 1; else HIGHPEAK = 0;
			if (midPWM > PWMcount)  MIDPEAK = 1; else  MIDPEAK = 0;
			if (lowPWM > PWMcount)  LOWPEAK = 1; else  LOWPEAK = 0;
	
			// PWMcount controls the range of the PWM
			PWMcount++;
			if (PWMcount > PWMSTEPS) PWMcount = 0;
	
			// Fade counter controls the fade speed (higher is longer)
			fadeCounter++;
			if (fadeCounter > 30) fadeCounter = 0;
		}
	}
}

// Reads an ADC convertor and returns 0-1023
int readMyADC(unsigned char ADCnumber)
{
  int result;

  // Select the desired ADC and start the conversion
  switch(ADCnumber)
  {
    case HIGHADC: ADCON0 = 0b00000001;  // Start the ADC conversion on AN0
                  break;
    case MIDADC:  ADCON0 = 0b00000101;  // Start the ADC conversion on AN1
                  break;
    case LOWADC:  ADCON0 = 0b00001001;  // Start the ADC conversion on AN2
                  break;
    case MAINADC: ADCON0 = 0b00001101;  // Start the ADC conversion on AN3
                  break;
  }

  // Start conversion
  ADCON0 |= 0x02;
  
  // Wait for the ADC conversion to complete
  while(GODONE);

  // Get the ADC result (1023 = +5v (5000 milliVolts)
  result = ADRESL;
  result += (ADRESH << 8);

  return result;
}