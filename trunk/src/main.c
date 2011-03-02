#include "../headers/typedef.h"
#include "../headers/io.h"
#include "../headers/sound.h"
		
//This is where the program starts
int main()
{
	//Initialize various subsystems
	BUTTONS_initialize( 0xFF );		//buttons
	LED_initialize( 0xFF );			//LED lamps
	IO_initialize_interrupts();		//interrupt controller
	SOUND_initialize();			//sound sub-system
	AUDIO_initialize();			//audio output controller

	while( true )
	{
		//TODO: bad infinite loop!
		//SOUND_progress_tracker();
	}
	
	return EXIT_SUCCESS;
}
