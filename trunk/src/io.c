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
__int_handler *rtc_int_handler();

/** PIOB interrupt (buttons) **/
__int_handler *piob_int_handler() 
{
	static bool button_release = false;

	//Interrupts to the switches
	DEBOUNCE();
	int buttons_pushed = piob->isr;

	//Only register on release, not on push down
	if( button_release )
	{
		if( buttons_pushed & 1 )
		{
			static bool pause = false;
			pause = !pause;
			if(pause) SOUND_pause(); else SOUND_play();
		}
		if( buttons_pushed & 2 ) SOUND_set_audio(0);
		if( buttons_pushed & 4 ) SOUND_set_audio(1);
		if( buttons_pushed & 8 ) SOUND_set_audio(2);
		if( buttons_pushed & 16 ) SOUND_set_audio(3);
		if( buttons_pushed & 32 ) SOUND_set_audio(4);
		if( buttons_pushed & 64 ) SOUND_set_wave_mode(SQUARE);
		if( buttons_pushed & 128 ) SOUND_set_wave_mode(TRIANGLE);
	}

	button_release = !button_release;
	
	return 0;
}

/** Digital to Analog Converter interrupt **/
__int_handler *dac_int_handler() 
{	
	short sound_data = SOUND_get_next_sample();
	
	pdac->SDR.channel0 = sound_data;	//Left
	pdac->SDR.channel1 = sound_data;	//Right

	//Enable next interrupt
	pdac->isr;
	return 0;
}

/** Real Time Counter clock interrupt **/
__int_handler *rtc_int_handler()
{	
	//progress the tracker
	SOUND_progress_tracker();

	//end interrupt
        psm->rtc_icr = true;
        return 0;
}


void RTC_set_top( int top )
{
	psm->rtc_top = top;
}

/** Enable or disable Real Time Counter interrupts **/
void RTC_set_enabled( bool enable )
{
        psm->RTC_CTRL.en = enable;
        psm->RTC_CTRL.psel = enable;
        psm->RTC_CTRL.pclr = enable;
	psm->RTC_CTRL.topen = enable;

	//Enable interrupts
	psm->rtc_ier = enable;

	//Flush any sound data to remove any background noise
	pdac->SDR.channel0 = 0;		//Left
	pdac->SDR.channel1 = 0;		//Right
}

/** Enable or disable sound output **/
void DAC_set_interrupt_enabled( bool enable )
{
	pdac->CR.en = enable;
	pdac->IER.tx_ready = enable;
}


/** This function initializes the digital to audio converter we are using **/
void DAC_initialize() 
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


	//Enable the Digital to Audio Converter (DAC) interrupts
	DAC_set_interrupt_enabled( true );

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
	register_interrupt( (__int_handler)( rtc_int_handler  ), AVR32_SM_RTC_IRQ / 32,  AVR32_SM_RTC_IRQ % 32,  INT1 );
	init_interrupts();
}

