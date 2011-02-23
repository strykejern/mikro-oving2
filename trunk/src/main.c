#include "../headers/typedef.h"
#include "../headers/sound.h"
#include "../headers/io.h"
		
int main( void )
{
	//Load music data
	sound_load_all_songs();

	//This is where the program starts
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );
	IO_initialize_interrupts();
	RTC_initialize();
	AUDIO_initialize();

	while( true )
	{
	}
	
	return EXIT_SUCCESS;
}
