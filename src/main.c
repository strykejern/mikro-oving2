#include "../headers/typedef.h"
#include "../headers/io.h"
#include "../headers/sound.h"

void sleep();

/** This is where the program starts **/
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
		sleep();
		//SOUND_progress_tracker();
	}
	
	return EXIT_SUCCESS;
}

/** This function sets the CPU into idle mode, stopping the main flow of the program until a interrupt happens **/
void sleep()
{
	//We need to use inline assembly to access this function
	asm( "SLEEP 0" );
}
