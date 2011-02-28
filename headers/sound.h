#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

/*
typedef enum note_enum
{
	INVALID_NOTE = -1,
	A = 440,	
	B = 494,
	C = 262,
	D = 294,
	E = 330,
	F = 349,
	G = 392,
} Note;
*/

typedef enum note_enum
{
	A = 0,	
	B,
	C,
	D,
	E,
	F,
	G,
	NOTE_NUM,

	//Special value
	SILENCE = -1
} Note;


typedef struct song_struct
{
	int length;			//actual length of the song
	int offset;			//current position in the song
	Note *array_start;		//pointer to array start
} Song;

/** Function prototypes, these are "Public" functions **/
Song* SOUND_get_current_song();
bool SOUND_set_current_song( const int songnum );
void SOUND_initialize();
int SOUND_get_note_frequency( const Note type );

#endif	/* SOUND_HEADER_INCLUDED */
