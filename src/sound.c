#include "../headers/typedef.h"
#include "../headers/sound.h"

//Music files
const Note ducktales_song[] = { 
A, B, C, D, E, SILENCE, E, F, SILENCE, F, SILENCE, F, E, D, SILENCE, D, SILENCE, D, E, SILENCE, E, B, SILENCE, B, SILENCE, B, A,
 }; 

//Private variables
static Song current_song;

/** Reset and set the current song **/
bool sound_set_current_song( const int songnum )
{
	//Invalid song?
	if( songnum < 0 || songnum >= 8 ) return false;

	current_song.current = 0;
	current_song.length = ARRAY_SIZE( ducktales_song );
	current_song.data = &ducktales_song;

	return true;
}


/** Get a pointer to the current song **/
Song* sound_get_current_song()
{
	return &current_song;
}

