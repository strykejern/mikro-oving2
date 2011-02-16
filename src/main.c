#include <stdlib.h>
#include "../headers/typedef.h"
#include "../headers/io.h"

		
int main( void )
{
	//This is where the program starts
//	AUDIO_initialize();
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );
	IO_initialize_interrupts();

	BITFIELD x = 1;

	while( true )
	{
	}
	
	return EXIT_SUCCESS;
}
