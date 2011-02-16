#include <stdlib.h>
#include <stdbool.h>
#include "../headers/io.h"

		
int main( void )
{
	BUTTONS_initialize( 0xFF );
	LED_initialize( 0xFF );

	//This is where the program starts
	while( true )
	{
		LED_enable( 0xFF );
	}
	
	return EXIT_SUCCESS;
}
