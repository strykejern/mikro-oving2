#include <math.h>

#include "../headers/typedef.h"
#include "../headers/sound.h"

//Music files
#include "../music/ducktales.mus"

//Private functions
static Song* parse_song( const char song_data[], const int array_size );
static void add_one_note( Song* psong, const char note );
static void precache_one_note( const Note frequency );

//Private variables
static Song* loaded_songs[8];
static short note_cache[NOTE_COUNT][SAMPLE_RATE];

void precache_one_note( const Note frequency )
{
	int sample, note = INVALID_NOTE;

	//figure out note number
	switch( frequency )
	{
		case A: note = 0; break;
		case B: note = 1; break;
		case C: note = 2; break;
		case D: note = 3; break;
		case E: note = 4; break;
		case F: note = 5; break;
		default: note = INVALID_NOTE; break;
	}

	//trap errors
	if( note == INVALID_NOTE ) return;

	//generate sine wave
	for( sample = 0; sample < SAMPLE_RATE; sample++ )
	{
		note_cache[note][sample] = sin( 2*M_PI * frequency * sample ) / SAMPLE_RATE;
	}
}

Song* parse_song( const char song_data[], const int array_size )
{
	Song* new_song;
	int i;

	//trap null pointers
	if( song_data == NULL ) return NULL;

	//Create new pointer
	new_song = (Song*) malloc( sizeof(Song) );
	new_song->length = 0;
	new_song->current = 0;

	//parse each letter
	for( i = 0; i < array_size; i++ )
	{
		add_one_note( new_song, song_data[i] );
	}

	return new_song;
} 


void add_one_note( Song* psong, const char note )
{
	Note add_note = INVALID_NOTE;

	if( psong->length >= MAX_SONG_LENGTH ) return;

	//Translate from Char to Note type
	switch( note )
	{
		case 'A': add_note = A; break;
		case 'B': add_note = B; break;
		case 'C': add_note = C; break;
		case 'D': add_note = D; break;
		case 'E': add_note = E; break;
		case 'F': add_note = F; break;
	}

	//Did we succeed? If so, add the note to the song
	if( add_note != INVALID_NOTE )
	{
		psong->data[psong->length++] = add_note;
	}
}

/** This function preloads every sine wave generated note into memory for later use **/
void sound_precache_all_notes()
{
	precache_one_note( A );
	precache_one_note( B );
	precache_one_note( C );
	precache_one_note( D );
	precache_one_note( E );
	precache_one_note( F );
}

/** Load all songs into memory **/
void sound_load_all_songs()
{
	loaded_songs[0] = parse_song( ducktales_song, sizeof(ducktales_song) );
}


/** Gets a loaded song from memory **/
Song* sound_get_song( const int songnum )
{
	if( songnum < 0 || songnum >= 8 ) return NULL;
	return loaded_songs[songnum];
}

/** Gets a loaded song from memory **/
short sound_get_note_sample( Note frequency, int sample )
{
	int note = INVALID_NOTE;

	//figure out note number
	switch( frequency )
	{
		case A: note = 0; break;
		case B: note = 1; break;
		case C: note = 2; break;
		case D: note = 3; break;
		case E: note = 4; break;
		case F: note = 5; break;
		default: note = INVALID_NOTE; break;
	}

	//trap errors
	if( note == INVALID_NOTE ) return 0;

	return note_cache[note][sample];
}
