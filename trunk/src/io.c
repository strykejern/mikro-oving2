#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include <math.h>

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

static short amplitude = 0x60B4;
static double frequency = 4<<8;

__int_handler *piob_int_handler() 
{
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	if( buttons_pushed & 1 ) frequency = 0;
	if( buttons_pushed & 128 ) frequency = rand();

	return 0;
}

__int_handler *dac_int_handler() 
{
	static double cycle = 0;
	static short sound = 0;

	cycle++;

	double result = sin(frequency*cycle);
	sound = (amplitude * result);	

	sound = 5.0 * sin(2.0*M_PI*2.0 * 4.0);
	
	pdac->SDR.channel0 = sound;
	pdac->SDR.channel1 = sound;
		
	//Enable next interrupt
	pdac->isr;
	return 0;
}
