#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

#include "../headers/sound.h"

/** Function prototypes, these are "Public" functions **/
void BUTTONS_initialize( const BITFIELD bits );
void LED_initialize( const BITFIELD bits );
void LED_set_enabled( const BITFIELD bits );
void AUDIO_initialize();
void IO_initialize_interrupts();
void RTC_initialize();

extern Song global_song;

#endif	/* IO_HEADER_INCLUDED */
