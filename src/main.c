#include <stdlib.h>
#include "../headers/typedef.h"
#include "../headers/io.h"

		
int main( void )
{
	//This is where the program starts
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );

	while( true )
	{
		LED_set_enabled( 0xFF );
	}
	
	return EXIT_SUCCESS;
}
