#include "../headers/typedef.h"
#include "../headers/sound.h"

//Music files
#include "../music/ducktales.mus"

//Private functions
static Song* parse_song( const char song_data[], const int array_size );
static void add_one_note( Song* psong, const char note );

//Private variables
static Song* loaded_songs[8];



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
	}

	//Did we succeed? If so, add the note to the song
	if( add_note != INVALID_NOTE )
	{
		psong->data[psong->length++] = add_note;
	}
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
