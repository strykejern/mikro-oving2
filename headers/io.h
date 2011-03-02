#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

/** Function prototypes, these are "Public" functions **/
void BUTTONS_initialize( const BITFIELD bits );
void IO_initialize_interrupts();

void LED_initialize( const BITFIELD bits );
void LED_set_enabled( const BITFIELD bits );

void RTC_initialize();
void RTC_set_interrupt_enabled( bool enable );
void RTC_set_top( int top );

void DAC_initialize();
void DAC_set_interrupt_enabled( bool enable );

#endif	/* IO_HEADER_INCLUDED */
