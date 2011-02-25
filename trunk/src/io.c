#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include <limits.h>
#include <math.h>

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
__int_handler *rtc_int_handler();
void sine_wave();
void square_wave();

static double amplitude = 0.25; //0x10B4;
static int frequency = A;

__int_handler *piob_int_handler() 
{
	static bool button_release = false;

	//Interrupts to the switches
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	//Only register on release, not on push down
	if( button_release )
	{
		if( buttons_pushed & 1 ) frequency = A;	
		if( buttons_pushed & 2 ) frequency = B;
		if( buttons_pushed & 4 ) frequency = C;
		if( buttons_pushed & 8 ) frequency = D;
		if( buttons_pushed & 16 ) frequency = E;
		if( buttons_pushed & 32 ) frequency = F;
		if( buttons_pushed & 64 ) frequency = G;
	}

	button_release = !button_release;
	
	return 0;
}

__int_handler *dac_int_handler() 
{	
	static int freq_clock = 0;
	static short wave = SHRT_MAX;
	static int note_length = 0;

	//Change note?
	note_length++;
	if( note_length >= 15000 )
	{
		Song *psong = sound_get_current_song(0);
		int note;
		note_length = 0;

		//get next note
		if( psong->current++ >= psong->length-1 ) psong->current = 0;
		frequency = psong->data[psong->current];

		//Display LED for which note we are playing
		switch( frequency )
		{
			case A: note = 1; break;
			case B: note = 2; break;
			case C: note = 4; break;
			case D: note = 8; break;
			case E: note = 16; break;
			case F: note = 32; break;
			case G: note = 64; break;
			default: note = SILENCE; break;
		}
		LED_set_enabled( note );
	}

	if( frequency != SILENCE )
	{
		//TODO: move this calculation to a loopkup table?
		int clock_time = ( 12000000LL / 256LL ) / frequency;		// (12MHz / 256) / frequency
	
		//Play square wave
		if( freq_clock++ >= clock_time )
		{
			freq_clock = 0;
			wave = -wave;
		}
	
		pdac->SDR.channel0 = wave;	//Left
		pdac->SDR.channel1 = wave;	//Right
	}

	//Enable next interrupt
	pdac->isr;
	return 0;
}

__int_handler *rtc_int_handler()
{
	//Enable next interrupt
	psm->rtc_icr = true;
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

void RTC_initialize()
{
	psm->RTC_CTRL.en = true;
	psm->RTC_CTRL.topen = 1;
	psm->RTC_CTRL.psel = 0;
	psm->RTC_CTRL.pclr = 1;
	psm->rtc_top = 1;
	
	psm->rtc_ier = true;
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
	//register_interrupt( (__int_handler)( rtc_int_handler  ), AVR32_SM_RTC_IRQ / 32,  AVR32_SM_RTC_IRQ % 32,  INT1 );
	init_interrupts();
}

