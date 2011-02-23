#include "../headers/typedef.h"
#include "../headers/sound.h"
#include "../headers/io.h"
		
int main( void )
{
	//This is where the program starts
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );
	IO_initialize_interrupts();
	RTC_initialize();
	AUDIO_initialize();
	
	LED_set_enabled( 0x8 );

	//Load music data
	sound_load_all_songs();

	while( true )
	{
	}
	
	return EXIT_SUCCESS;
}
