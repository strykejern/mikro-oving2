#include <limits.h>

#include "../headers/typedef.h"
#include "../headers/sound.h"
#include "../headers/io.h"

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
static Song audio_list[8];

static Note current_note = SILENCE;	//Current note being played (SILENCE if there is none)
static WAVE_MODE wave_mode = SQUARE;

static short amplitude = SHRT_MAX/4;	//25% volume

//Private functions
void precache_notes();
short square_wave();
short triangle_wave();

/** Reset and set the current song **/
bool SOUND_set_current_song( const int songnum )
{
	//Invalid song?
	if( songnum < 0 || songnum >= 8 ) return false;

	current_song = songnum;
	audio_list[current_song].offset = 0;

	return true;
}

/** Initializes the sound subsystem **/
void SOUND_initialize()
{
	//Load one song
	audio_list[1].length = ARRAY_SIZE( sample_song );
	audio_list[1].array_start = &sample_song[0];

	//Load one song
	audio_list[2].length = ARRAY_SIZE( ducktales_song );
	audio_list[2].array_start = &ducktales_song[0];

	//Load one song
	audio_list[3].length = ARRAY_SIZE( beatles_song );
	audio_list[3].array_start = &beatles_song[0];

	//Load one song
	audio_list[3].length = ARRAY_SIZE( test_song );
	audio_list[3].array_start = &test_song[0];
	
	//Finish up
	precache_notes();
	SOUND_set_current_song(0);
}

/** This function progresses a song to the next note **/
void SOUND_progress_tracker()
{
	static int note_length = 0;

	//Change to next note?
	note_length++;
	if( note_length >= 15000 )
	{
		BITFIELD bits;
		Song *psong = &audio_list[current_song];		

		//Reset counter
		note_length = 0;

		//get next note
		if( psong->offset++ >= psong->length-1 ) psong->offset = 0;
		current_note = (Note) *(psong->array_start + psong->offset);

		//Display LED for which note we are playing
		switch( current_note )
		{
			case A: bits = 1; break;
			case B: bits = 2; break;
			case C: bits = 4; break;
			case D: bits = 8; break;
			case E: bits = 16; break;
			case F: bits = 32; break;
			case G: bits = 64; break;
			default: bits = 0; break;
		}
		LED_set_enabled( bits );
	}
}

void SOUND_set_sound_mode( WAVE_MODE mode )
{
	wave_mode = mode;
}

/** This function gets the next audio sample **/
short SOUND_get_next_sample()
{
	//TODO: move this function to RTC
	SOUND_progress_tracker();

	//Do we have something to play?
	if( current_note == SILENCE ) return 0;

	if( wave_mode == TRIANGLE ) return triangle_wave();
	if( wave_mode == SQUARE   ) return square_wave();

	//No sound
	return 0;
}

short square_wave()
{
	static int freq_clock = 0;
	static bool rising = false;

	//Play square wave
	if( freq_clock++ >= note_precache[current_note] )
	{
		freq_clock = 0;
		rising = !rising;
	}

	return rising ? amplitude : -amplitude;
}

short triangle_wave()
{
	static int cycle = 0;
	static bool rising = true;
	
	if (rising)
	{
		cycle++;
		if (cycle >= note_precache[current_note]) rising = false;
	}
	else
	{
		cycle--;
		if (cycle <= -note_precache[current_note]) rising = true;
	}
	
	return (cycle * (int)amplitude) / note_precache[current_note];
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
