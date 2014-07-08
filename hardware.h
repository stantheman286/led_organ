/************************************************************************
	hardware.h

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

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

// Fosc frequency (48 Mhz)
#define _XTAL_FREQ 48000000

// Common useful definitions
#define ON 	1
#define OFF	0
#define	WRITE	0
#define READ	1
#define FALSE	0
#define TRUE	1

// Peak indicator LEDs
#define HIGHPEAK	RB2
#define MIDPEAK		RB3
#define LOWPEAK		RB4

// LED string outputs
#define HIGHSTRING	RC0
#define MIDSTRING	RC1
#define LOWSTRING	RC2

// Toggle Switch pins
#define SWITCHLOW	RC5
#define SWITCHHIGH	RC4

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

#endif