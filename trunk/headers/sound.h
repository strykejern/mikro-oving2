#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

#define MAX_SONG_LENGTH 512

typedef enum note_enum
{
	INVALID_NOTE = -1,
	A = 1,
	B = 2,
	C = 3,
	D = 4
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
