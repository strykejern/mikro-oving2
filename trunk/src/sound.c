#include "../headers/typedef.h"
#include "../headers/sound.h"

//Music files
Note sample_song[] = { 
A, B, C, D, E, SILENCE, E, F, SILENCE, F, SILENCE, F, E, D, SILENCE, D, SILENCE, D, E, SILENCE, E, B, SILENCE, B, SILENCE, B, A,
 };

Note ducktales_song[] = { 

//Verse 1
E, D, A, E, D, A, D, A, F, B, A, B,

//Chorus
E, C, D, E, C, D, E

 }; 

Note beatles_song[] = { 

//Verse 1
D, D, E, E, C, G, G, F, D, E, D, C, D, E, E, E, A, G, F, G, E, D

 }; 


Note test_song[] = { 
 C, SILENCE, C, SILENCE, C, B, B, B, SILENCE, B, SILENCE, B, SILENCE, B, A, A, A,
 C, SILENCE, C, SILENCE, C, B, B, B, A, A, D, E, D, E, F, E, D, C
 }; 

//Private variables
static int current_song;
static Note note_precache[NOTE_NUM];
static Song songs_loaded[8];

//Private functions
void precache_notes();

/** Reset and set the current song **/
bool SOUND_set_current_song( const int songnum )
{
	//Invalid song?
	if( songnum < 0 || songnum >= 8 ) return false;

	current_song = songnum;
	songs_loaded[current_song].offset = 0;

	return true;
}


/** Get a pointer to the current song **/
Song* SOUND_get_current_song()
{
	return &songs_loaded[current_song];
}

int SOUND_get_note_frequency( const Note type )
{
	//Trap invalid notes
	if( type <= SILENCE || type >= NOTE_NUM ) return SILENCE;

	return note_precache[type];
}

/** Initializes the sound subsystem **/
void SOUND_initialize()
{
	//Load one song
	songs_loaded[1].length = ARRAY_SIZE( sample_song );
	songs_loaded[1].array_start = &sample_song[0];

	//Load one song
	songs_loaded[2].length = ARRAY_SIZE( ducktales_song );
	songs_loaded[2].array_start = &ducktales_song[0];

	//Load one song
	songs_loaded[3].length = ARRAY_SIZE( beatles_song );
	songs_loaded[3].array_start = &beatles_song[0];

	//Load one song
	songs_loaded[3].length = ARRAY_SIZE( test_song );
	songs_loaded[3].array_start = &test_song[0];
	
	//Finish up
	precache_notes();
	SOUND_set_current_song(0);
}

/** Precache note frequencies in a lookup table so we don't have to recalculate these each interrupt**/
void precache_notes()
{
	//Octave 4 notes
	note_precache[A] = ( 12000000LL / 256LL ) / 440;
	note_precache[B] = ( 12000000LL / 256LL ) / 494;
	note_precache[C] = ( 12000000LL / 256LL ) / 523;
	note_precache[D] = ( 12000000LL / 256LL ) / 587;
	note_precache[E] = ( 12000000LL / 256LL ) / 659;
	note_precache[F] = ( 12000000LL / 256LL ) / 784;
	note_precache[G] = ( 12000000LL / 256LL ) / 880;
}
