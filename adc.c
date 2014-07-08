/************************************************************************
	adc.c

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

// Global includes
#include <htc.h>

// Local includes
#include "hardware.h"
#include "adc.h"

// Reads an ADC convertor and returns 0-1023
int readADC(unsigned char ADCnumber)
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

  // Wait for the ADC conversion to complete
  while(GODONE);

  // Get the ADC result (1023 = +5v (5000 milliVolts)
  result = ADRESL;
  result += (ADRESH << 8);

  return result;
}
