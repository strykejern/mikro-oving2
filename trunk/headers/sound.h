#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

//Constants
#define MAX_SONG_LENGTH 64		//Max notes per song
#define SAMPLE_RATE 1000		//Number of samples per note

typedef enum note_enum
{
	INVALID_NOTE = -1,
	A = 273302,	//Tone A = 73.179 Hz
	B = 234261,	//Tone B = 85.375 Hz
	C = 204927,	//Tone C = 97.572 Hz
	D = 182202,	//Tone D = 109.768 Hz
	E = 163981,	//Tone E = 121.965 Hz
	F = 149075,	//Tone F = 134.161 Hz
	NOTE_COUNT = 6	//Number of different tones
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
void sound_precache_all_notes();
short sound_get_note_sample( Note note, int sample );

#endif	/* SOUND_HEADER_INCLUDED */
