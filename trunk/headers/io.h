//Header guard to prevent recursive includes
#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

#include <avr32/ap7000.h>
#include <sys/interrupts.h>

//Function prototypes, these are "Public" functions
void BUTTONS_initialize( const int bits );
void LED_initialize( const int bits );
void LED_set_enabled( const int bits );

#endif	/* IO_HEADER_INCLUDED */
