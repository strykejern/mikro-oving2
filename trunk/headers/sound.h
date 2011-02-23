#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

#define MAX_SONG_LENGTH 64

typedef enum note_enum
{
	INVALID_NOTE = -1,
	A = 1,		//Tone A = 73.179 Hz
	B = 2,		//Tone B = 85.375 Hz
	C = 3,		//Tone C = 97.572 Hz
	D = 4,		//Tone D = 109.768 Hz
	E = 5,		//Tone E = 121.965 Hz
	F = 6		//Tone F = 134.161 Hz
} Note;

typedef struct song_struct
{
	int length;			//actual length of the song
	int current;			//current position in the song
	Note data[MAX_SONG_LENGTH];	//sound data
} Song;

/** Function prototypes, these are "Public" functions **/
void sound_load_all_songs();
Song* sound_get_song( int songnum );

#endif	/* SOUND_HEADER_INCLUDED */
