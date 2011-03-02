#include <limits.h>

#include "../headers/typedef.h"
#include "../headers/sound.h"
#include "../headers/io.h"


//Music files
int sample_song[] = { 
 X, 500,
 C, 2400,
 X, 500,
 D, 2400,
 X, 500,
 E, 2400,
 X, 500,
 F, 2400,
 X, 500,
 G, 4800,
 X, 500,
 G, 4800,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 G, 6000,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 E, 4800,
 X, 500,
 E, 4800,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 C, 10000
 };




int ducktales_song[] = { 

//Verse 1
E, 1200,
D, 1200,
A, 1200,
E, 1200,
D, 1200,
A, 1200,
D, 1200,
A, 1200,
F, 1200,
B, 1200,
A, 1200,
B, 1200

}; 

int beatles_song[] = { 

 

 }; 


int test_song[] = { 
 C, X, C, X, C, B, B, B, X, B, X, B, X, B, A, A, A,
 C, X, C, X, C, B, B, B, A, A, D, E, D, E, F, E, D, C
 }; 

//Private variables
static int current_song;
static int note_precache[NOTE_NUM];
static Audio audio_list[8];

static Note current_note = SILENCE;	//Current note being played (SILENCE if there is none)
static WAVE_MODE wave_mode = SQUARE;

static short amplitude = SHRT_MAX/4;	//25% volume

//Private functions
void _precache_notes();
short _square_wave();
short _triangle_wave();

/** Reset and set the current song **/
bool SOUND_set_audio( const int songnum )
{
	//Invalid song?
	if( songnum < 0 || songnum >= 8 ) return false;

	current_song = songnum;
	audio_list[current_song].offset = 0;

	SOUND_play();

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
	_precache_notes();
	SOUND_stop();
}

void SOUND_pause()
{
//	DAC_set_interrupt_enabled(false);
	RTC_set_interrupt_enabled(false);
	current_note = SILENCE;
}

void SOUND_play()
{
//	DAC_set_interrupt_enabled(true);
	RTC_set_interrupt_enabled(true);
}

void SOUND_stop()
{
//	DAC_set_interrupt_enabled(false);
	RTC_set_interrupt_enabled(false);
	audio_list[current_song].offset = 0;
	current_note = SILENCE;
}

/** This function progresses a song to the next note **/
void SOUND_progress_tracker()
{
	BITFIELD bits;
	Audio *paudio = &audio_list[current_song];		

	//Progress to next note
	paudio->offset += 2;

	//end of audio?
	if( paudio->offset >= paudio->length-1 )
	{
		SOUND_stop();
		return;
	}

	//Set the duration of this note
	RTC_set_top( (int) *(paudio->array_start + paudio->offset + 1) );

	//get next note
	current_note = (Note) *(paudio->array_start + paudio->offset);
	
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

void SOUND_set_wave_mode( WAVE_MODE mode )
{
	wave_mode = mode;
}

/** This function gets the next audio sample **/
short SOUND_get_next_sample()
{
	//Do we have something to play?
	if( current_note == X ) return 0;

	if( wave_mode == TRIANGLE ) return _triangle_wave();
	if( wave_mode == SQUARE   ) return _square_wave();

	//No sound
	return 0;
}

short _square_wave()
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

short _triangle_wave()
{
	static int cycle = 0;
	static bool rising = true;
	
	int note = note_precache[current_note]>>1;
	
	if (rising)
	{
		cycle++;
		if (cycle >= note)
		{
			rising = false;
			cycle = note;
		}
	}
	else
	{
		cycle--;
		if (cycle <= -note)
		{
			rising = true;
			cycle = -note;
		}
	}

	return (cycle * amplitude * 4) / note;
}

/** Precache note frequencies in a lookup table so we don't have to recalculate these each interrupt**/
void _precache_notes()
{
	//Octave 0 notes
	note_precache[C] = ( 12000000LL / 256LL ) / 523;
	note_precache[D] = ( 12000000LL / 256LL ) / 587;
	note_precache[E] = ( 12000000LL / 256LL ) / 659;
	note_precache[F] = ( 12000000LL / 256LL ) / 698;
	note_precache[G] = ( 12000000LL / 256LL ) / 784;
	note_precache[A] = ( 12000000LL / 256LL ) / 880;
	note_precache[B] = ( 12000000LL / 256LL ) / 988;
}
