#include "../headers/typedef.h"
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

        global_song.length = 7;
        global_song.data[0] = A;
        global_song.data[1] = B;
        global_song.data[2] = C;
        global_song.data[3] = A;
        global_song.data[4] = D;
        global_song.data[5] = C;
        global_song.data[6] = B;

	while( true )
	{
	}
	
	return EXIT_SUCCESS;
}
