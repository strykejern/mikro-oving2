#include "../headers/typedef.h"
#include "../headers/io.h"
#include "../headers/sound.h"
		
int main()
{
	sound_set_current_song(0);

	//This is where the program starts
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );
	IO_initialize_interrupts();
	AUDIO_initialize();

	while( true )
	{
	}
	
	return EXIT_SUCCESS;
}
