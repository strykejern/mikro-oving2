#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

//Constants
#define MAX_SONG_LENGTH 64		//Max notes per song
#define SAMPLE_RATE 1000		//Number of samples per note

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

//Octav 4
typedef enum note_enum
{
	INVALID_NOTE = -1,
	SILENCE = 0,
	A = 440,	
	B = 494,
	C = 523,
	D = 587,
	E = 659,
	F = 784,
	G = 880,
} Note;


typedef struct song_struct
{
	int length;			//actual length of the song
	int current;			//current position in the song
	Note (*data)[];			//sound data
} Song;

/** Function prototypes, these are "Public" functions **/
Song* sound_get_current_song();
bool sound_set_current_song( const int songnum );

#endif	/* SOUND_HEADER_INCLUDED */
