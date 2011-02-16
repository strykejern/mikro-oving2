#include <stdlib.h>
#include "../headers/typedef.h"
#include "../headers/io.h"

		
int main( void )
{
	//This is where the program starts
	AUDIO_initialize();
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );

	while( true )
	{
		LED_set_enabled( 0xFF );
//		SLEEP( 1000 );
	}
	
	return EXIT_SUCCESS;
}
