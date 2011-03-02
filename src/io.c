#include <avr32/ap7000.h>
#include <sys/interrupts.h>

#include "../headers/typedef.h"
#include "../headers/io.h"
#include "../headers/sound.h"


//Input/Output control
static volatile avr32_pio_t *piob = &AVR32_PIOB;	//Buttons
static volatile avr32_pio_t *pioc = &AVR32_PIOC;	//LED

static volatile avr32_dac_t *pdac = &AVR32_DAC;		//Digital to Audio Converter
static volatile avr32_sm_t *psm   = &AVR32_SM;		//Power Manager

//Private functions
__int_handler *piob_int_handler();
__int_handler *dac_int_handler();


__int_handler *piob_int_handler() 
{
	static bool button_release = false;

	//Interrupts to the switches
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	//Only register on release, not on push down
	if( button_release )
	{
		if( buttons_pushed & 1 ) SOUND_set_current_song(0);	
		if( buttons_pushed & 2 ) SOUND_set_current_song(1);
		if( buttons_pushed & 4 ) SOUND_set_current_song(2);
		if( buttons_pushed & 8 ) SOUND_set_current_song(3);
		if( buttons_pushed & 16 ) SOUND_set_current_song(4);
		if( buttons_pushed & 32 ) SOUND_set_current_song(5);
		if( buttons_pushed & 64 ) SOUND_set_current_song(6);
	}

	button_release = !button_release;
	
	return 0;
}

__int_handler *dac_int_handler() 
{	
	short sound_data = SOUND_get_next_sample();
	
	if( sound_data )
	{
		pdac->SDR.channel0 = sound_data;	//Left
		pdac->SDR.channel1 = sound_data;	//Right
	}

	//Enable next interrupt
	pdac->isr;
	return 0;
}

/** This function initializes the audio system we are using **/
void AUDIO_initialize() 
{
	//NOTE! Interrupts have to be initialized before this can work!

	//Setup the ABDAC clock to use out PLL clock
	volatile avr32_sm_pm_gcctrl_t  *clock =  &(psm->PM_GCCTRL[6]);
	clock->cen = true;	
	clock->oscsel = 1;	//12 MHz
	
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

/** Initialize interrupts **/
void IO_initialize_interrupts()
{
	set_interrupts_base( (void *) AVR32_INTC_ADDRESS );
	register_interrupt( (__int_handler)( piob_int_handler ), AVR32_PIOB_IRQ / 32, AVR32_PIOB_IRQ % 32, INT0 );
	register_interrupt( (__int_handler)( dac_int_handler  ), AVR32_DAC_IRQ / 32,  AVR32_DAC_IRQ % 32,  INT0 );
	init_interrupts();
}

