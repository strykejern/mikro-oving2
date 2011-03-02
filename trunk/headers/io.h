#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

/** Function prototypes, these are "Public" functions **/
void BUTTONS_initialize( const BITFIELD bits );
void DAC_initialize();
void RTC_initialize();
void IO_initialize_interrupts();

void LED_initialize( const BITFIELD bits );
void LED_set_enabled( const BITFIELD bits );

#endif	/* IO_HEADER_INCLUDED */
