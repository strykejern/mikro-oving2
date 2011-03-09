#ifndef SOUND_HEADER_INCLUDED
#define SOUND_HEADER_INCLUDED

#define MAX_AUDIO 5			//Maximum number of sound effects supported

//The various wave types we can play
typedef enum wave_mode_enum
{
	TRIANGLE = 0,
	SQUARE,
	SAWTOOTH
} WAVE_MODE;

typedef enum note_enum {
	C = ( 12000000LL / 256LL ) / 523,
	D = ( 12000000LL / 256LL ) / 587,
	E = ( 12000000LL / 256LL ) / 659,
	F = ( 12000000LL / 256LL ) / 698,
	G = ( 12000000LL / 256LL ) / 784,
	A = ( 12000000LL / 256LL ) / 880,
	B = ( 12000000LL / 256LL ) / 988,

	X = 0,					//Silence
	SILENCE = X,				//alias for Silence
	EOT = -1				//end of track
} Note;


//The various notes we can play
/*typedef enum note_enum
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
	SILENCE = -1,			//Silence
	X = SILENCE,			//Alias for Silence
	STOP = -2			//End of track
} Note;*/


//Data structure for an audio track
typedef struct audio_struct
{
	int length;			//actual length of the audio track
	int offset;			//current position in the audio track
	int *array_start;		//pointer to array start
} Audio;

/** Function prototypes, these are "Public" functions **/
void SOUND_initialize();
void SOUND_next_wave_mode();
bool SOUND_play_audio( const int number );

void SOUND_progress_tracker();
short SOUND_get_next_sample();

void SOUND_stop();
void SOUND_play();
void SOUND_pause();

int SOUND_change_volume( const int modify );

#endif	/* SOUND_HEADER_INCLUDED */
