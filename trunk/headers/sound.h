#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

typedef enum tone_enum
{
	A = 50,
	B = 100,
	C = 150,
	D = 200
} Tone;

typedef struct song_struct
{
	int length;		//actual length of the song
	int current;		//current position in the song
	Tone data[512];		//sound data
} Song;

#endif	/* SOUND_HEADER_INCLUDED */
