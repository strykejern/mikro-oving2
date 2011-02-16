#include <stdlib.h>
#include <avr32/ap7000.h>
#include <sys/interrupts.h>

#include "../headers/typedef.h"
#include "../headers/io.h"

//Input/Output control
static volatile avr32_pio_t *piob = &AVR32_PIOB;	//Buttons
static volatile avr32_pio_t *pioc = &AVR32_PIOC;	//LED

static volatile avr32_dac_t *pdac = &AVR32_DAC;		//Digital to Audio Converter
static volatile avr32_sm_t *psm   = &AVR32_SM;		//Power Manager

__int_handler *piob_int_handler();
__int_handler *dac_int_handler();


/** This function initializes the audio system we are using **/
void AUDIO_initialize() 
{
	//NOTE! Interrupts have to be initialized before this can work!

	//Initialize clock frequency for output sound
	psm->pm_gcctrl[6] = 0x0005; //todo: data

	//Setup PIO to use ABDAC
	piob->PDR.p20 = true;		//We are no longer the controller
	piob->PDR.p21 = true;
	piob->ASR.p20 = true;		//Tell it that Peripheral 1 is now the controller
	piob->ASR.p21 = true;


	//Enable the Digital to Audio Converter (DAC)
	pdac->CR.en = true;
	pdac->IER.tx_ready = true;

	//Now we simply write audio samples to: pdac->SDR
}

/** This function initializes the buttons **/
void BUTTONS_initialize( const BITFIELD bits ) 
{
	piob->odr = 0xFF;		//Disable output on buttons

	//Enable switches
	piob->per = bits;		//Register enable
	piob->ier = bits;		//Interrupt enable
	piob->puer = bits;		//Pullup enable

	//Disable everything that isn't enabled
	piob->pdr = ~bits;
	piob->idr = ~bits;
}

/** This function initializes the LED lamps **/
void LED_initialize( const BITFIELD bits ) 
{
	//Enable LED
	pioc->per = bits;		//Register enable
	pioc->oer = bits;		//Output enable

	//Disable leds that arent used
	pioc->pdr = ~bits;
}


/** Use this function to enable the LED specified in the BITFIELD **/
void LED_set_enabled( const BITFIELD bits ) 
{
	pioc->codr = ~bits;
	pioc->sodr = bits;
}

/** Initialize interrupts, do last **/
void IO_initialize_interrupts()
{
	set_interrupts_base( (void *) AVR32_INTC_ADDRESS );
	register_interrupt( (__int_handler)( piob_int_handler ), AVR32_PIOB_IRQ / 32, AVR32_PIOB_IRQ % 32, INT0 );
	register_interrupt( (__int_handler)( dac_int_handler ), AVR32_DAC_IRQ / 32, AVR32_DAC_IRQ % 32, INT0 );
	init_interrupts();
}

static short sound = 0x60FB;
static short limit = 100;

__int_handler *piob_int_handler() 
{
	DEBOUNCE();

	int status = piob->isr;

	LED_set_enabled( status );
	limit = status << 2;

	return 0;
}

__int_handler *dac_int_handler() 
{
	static bool swap = false;
	static int cycle = 1;

	cycle++;
	if( cycle >= limit ) cycle = 1;

	pdac->SDR.channel0 = swap ? -sound/cycle : sound/cycle;
	pdac->SDR.channel1 = swap ? sound/cycle : -sound/cycle;
	swap = !swap;

		
	//Enable next interrupt
	pdac->isr;
	return 0;
}
