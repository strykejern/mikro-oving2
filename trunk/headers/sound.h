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

typedef enum wave_mode_enum
{
	TRIANGLE = 0,
	SQUARE
} WAVE_MODE;


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
	X = -1,
	SILENCE = X
} Note;


typedef struct song_struct
{
	int length;			//actual length of the song
	int offset;			//current position in the song
	int *array_start;		//pointer to array start
} Song;

/** Function prototypes, these are "Public" functions **/
void SOUND_initialize();
void SOUND_set_sound_mode( WAVE_MODE mode );
bool SOUND_set_current_song( const int songnum );

void SOUND_progress_tracker();
short SOUND_get_next_sample();

void SOUND_stop();
void SOUND_play();
void SOUND_pause();

#endif	/* SOUND_HEADER_INCLUDED */
