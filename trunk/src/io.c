#include "io.h"

//Input/Output control
static volatile avr32_pio_t *piob = &AVR32_PIOB;	//Buttons
static volatile avr32_pio_t *pioc = &AVR32_PIOC;	//LED


void BUTTONS_initialize( const int bits )
{
	//Enable switches
	piob->per = bits;
	piob->ier = bits;
	piob->puer = bits;

	//Disable everything that isn't enabled
	piob->pdr = ~bits;
	piob->idr = ~bits;
}

void LED_initialize( const int bits )
{
	//Enable LED
	pioc->per = bits;
	pioc->oer = bits;

	//Disable leds that arent used
	pioc->pdr = ~bits;
}

void LED_enable( const int bits )
{
	pioc->codr = bits;
}
