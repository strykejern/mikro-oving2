#include <avr32/ap7000.h>
#include <sys/interrupts.h>

#include "../headers/typedef.h"
#include "../headers/io.h"
#include "../headers/sound.h"

#include <math.h>	//TODO: REMOVE THIS LINE

//Input/Output control
static volatile avr32_pio_t *piob = &AVR32_PIOB;	//Buttons
static volatile avr32_pio_t *pioc = &AVR32_PIOC;	//LED

static volatile avr32_dac_t *pdac = &AVR32_DAC;		//Digital to Audio Converter
static volatile avr32_sm_t *psm   = &AVR32_SM;		//Power Manager

//Private functions
__int_handler *piob_int_handler();
__int_handler *dac_int_handler();
__int_handler *rtc_int_handler();
void sine_wave();
void square_wave();


/** This function initializes the audio system we are using **/
void AUDIO_initialize() 
{
	//NOTE! Interrupts have to be initialized before this can work!

	//Initialize clock frequency for output sound
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

void RTC_initialize()
{
	psm->RTC_CTRL.en = 1;
	psm->RTC_CTRL.topen = 1;
	psm->RTC_CTRL.psel = 0;
	psm->RTC_CTRL.pclr = 1;
	psm->rtc_top = 1;
	
	psm->rtc_ier = 1;
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
	register_interrupt( (__int_handler)( dac_int_handler  ), AVR32_DAC_IRQ / 32,  AVR32_DAC_IRQ % 32,  INT0 );
	register_interrupt( (__int_handler)( rtc_int_handler  ), AVR32_SM_RTC_IRQ / 32,  AVR32_SM_RTC_IRQ % 32,  INT1 );
	init_interrupts();
}

static double amplitude = 0.25*65535; //0x10B4;
static double frequency = 1;

__int_handler *piob_int_handler() 
{
	static bool push = false;

	//Interrupts to the switches
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	if( push )
	{
		if( buttons_pushed & 1 ) frequency++;	
		if( buttons_pushed & 2 ) frequency--;	
	//	if( buttons_pushed & 4 ) frequency = B;	
	//	if( buttons_pushed & 8 ) frequency = C;	
	//	if( buttons_pushed & 16) frequency = D;
	}

	push = !push;
	LED_set_enabled( frequency );

	return 0;
}

__int_handler *dac_int_handler() 
{	
/*	static int sample = 0;
	Song *psong = sound_get_song(0);
	short sound;

	sound = sin( 2*M_PI * psong->data[psong->current] * sample ) / SAMPLE_RATE;
//	sound = sound_get_note_sample( psong->data[psong->current], sample++ );
	sample %= SAMPLE_RATE;
	sound = A*sample;

	pdac->SDR.channel0 = sound;	//Left
	pdac->SDR.channel1 = sound;	//Right		
*/
	//Enable next interrupt
	pdac->isr;
	return 0;
}

__int_handler *rtc_int_handler()
{
/*	static int note_countdown = 0;

	//play next note?
	if( note_countdown++ > 8000 )
	{
		Song *psong = sound_get_song(0);
		note_countdown = 0;

		//next note
		psong->current++;
		psong->current %= psong->length;
	}
*/

	//Enable next interrupt
	psm->rtc_icr = true;
	return 0;
}


void square_wave()
{
	static int wave = 1;
	static bool flip = true;
	
	flip = !flip;
	
	if (flip) wave *= -1;
	
	pdac->SDR.channel0 = amplitude * wave;	//Left
	pdac->SDR.channel1 = amplitude * wave;	//Right
}


















