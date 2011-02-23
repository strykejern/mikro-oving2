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

//Private functions
__int_handler *piob_int_handler();
__int_handler *dac_int_handler();
void sine_wave();
void square_wave();


/** This function initializes the audio system we are using **/
void AUDIO_initialize() 
{
	//NOTE! Interrupts have to be initialized before this can work!

	//Initialize clock frequency for output sound
//	psm->pm_gcctrl[6] = 12.288; //This value must be 256 times the frequency of the sample rate (48kHz = 12.288 MHz)
	volatile avr32_sm_pm_gcctrl_t  *clock =  &(psm->PM_GCCTRL[6]);
	clock->cen = true;
	clock->oscsel = 0; 	//20 MHz
	
	
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

static short amplitude = 0x10B4;
static int frequency = 300;

__int_handler *piob_int_handler() 
{
	//Interrupts to the switches
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	if( buttons_pushed & 1 ) frequency = 1;	//C4
	if( buttons_pushed & 2 ) frequency = 50;	//D4
	if( buttons_pushed & 4 ) frequency = 100;	//F4
	if( buttons_pushed & 8 ) frequency = 150;	//G5
	if( buttons_pushed & 16) frequency = 200;	//G#

	return 0;
}

__int_handler *dac_int_handler() 
{
	square_wave(frequency);
		
	//Enable next interrupt
	pdac->isr;
	return 0;
}


void sine_wave()
{
	//Interrupts for the digital to audio converter
	static int sample = 0;
	short sound = amplitude * sin(M_PI*2*frequency*sample);
	
	sample++;
	sample %= 8;
	
	pdac->SDR.channel0 = sound;	//Left
	pdac->SDR.channel1 = sound;	//Right
}

void square_wave(int frequency)
{
	static int wave = 1;
	static int cycle = 0;
	cycle++;
	
	if (cycle > frequency){
		cycle = 0;
		wave *= -1;
	}
	
	pdac->SDR.channel0 = amplitude * wave;	//Left
	pdac->SDR.channel1 = amplitude * wave;	//Right
}


















