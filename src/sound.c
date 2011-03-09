#include <limits.h>

#include "../headers/typedef.h"
#include "../headers/sound.h"
#include "../headers/io.h"
#include "../headers/sound_effects.h"

//Private variables
static int current_song;		//Current audio track being played
static Audio audio_list[MAX_AUDIO];	//Audio tracks loaded into memory

static Note current_note = SILENCE;	//Current note being played (SILENCE if there is none)
static WAVE_MODE wave_mode = SQUARE;	//Wave output type

static short amplitude; 		//Actual amplitude (-65535 to 65535)
static int volume;			//Abstract sound volume (0 to 8)
static int freq_clock = 0;		//Frequency clock timer

//Private functions
static short 	square_wave();
static short 	triangle_wave();
static short	sawtooth_wave();
static int 	load_audio( int array[], const int length );

/** Reset and set the current song **/
bool SOUND_play_audio( const int songnum )
{
	//Invalid audio?
	if( songnum < 0 || songnum >= MAX_AUDIO ) return false;

	//Reset track
	current_song = songnum;
	audio_list[current_song].offset = 0;

	//Load the first note
	current_note = (int) *(audio_list[current_song].array_start );
	RTC_set_top( (int) *(audio_list[current_song].array_start + 1) );

	SOUND_play();

	return true;
}

/** Increase or decrease sound volume (from 0 to 8) **/
int SOUND_change_volume( const int modify )
{
	BITFIELD set_led = 0;
	int i;

	//Apply the change
	volume += modify;
	if( volume < 0 ) 	volume = 0;
	else if (volume > 8 ) 	volume = 8;

	//Calculate amplitude from volume level
	amplitude = (volume == 0) ? 0 : SHRT_MAX / ( 17 - (volume*2) );

	//Display volume on the LEDs
	for( i = 0; i < volume; i++ ) set_led |= 1<<i;
	LED_set_enabled( set_led );

	return volume;
}

/** Initializes the sound subsystem **/
void SOUND_initialize()
{
	//Load paddle left
	load_audio( paddle_left, ARRAY_SIZE(paddle_left) );

	//Load paddle right
	load_audio( paddle_right, ARRAY_SIZE(paddle_right) );

	//Load paddle miss
	load_audio( paddle_fail, ARRAY_SIZE(paddle_fail) );

	//Load victory song
	load_audio( victory_song, ARRAY_SIZE(victory_song) );
	
	//Load intro song
	load_audio( pong_song, ARRAY_SIZE(pong_song) );

	//Finish up
	SOUND_stop();

	//so that amplitude is calculated
	volume = 5;
	SOUND_change_volume( 0 );
}

/** Loads an audio track into the first free audio slot found **/
int load_audio( int array[], const int length )
{
	static int slot = 0;

	//Don't add invalid songs
	if( slot >= MAX_AUDIO ) return -1;
	if( (length & 1) != 0 ) return -1;

	audio_list[slot].length = length;
	audio_list[slot].array_start = &array[0];

	return slot++;
}

/** Stop playing sound without resetting tracker **/
void SOUND_pause()
{
//	DAC_set_interrupt_enabled(false);
	RTC_set_enabled(false);
	current_note = SILENCE;
}

/** Resume or start playing sound **/
void SOUND_play()
{
	Audio *paudio = &audio_list[current_song];
	current_note = (Note) *(paudio->array_start + paudio->offset);
	RTC_set_enabled(true);
//	DAC_set_interrupt_enabled(true);
}

/** Stop playing the current sound and reset tracker position **/
void SOUND_stop()
{
//	DAC_set_interrupt_enabled(false);
	RTC_set_enabled(false);
	audio_list[current_song].offset = 0;
	current_note = EOT;
}

/** This function progresses a song to the next note **/
void SOUND_progress_tracker()
{
	BITFIELD bits;
	Audio *paudio = &audio_list[current_song];		

	//Progress to next note
	paudio->offset += 2;

	//end of audio?
	if( paudio->offset >= paudio->length-1 )
	{
		SOUND_stop();
		LED_set_enabled( 0 );
		return;
	}

	//Set the duration of this note
	RTC_set_top( (int) *(paudio->array_start + paudio->offset + 1) );

	//get next note
	current_note = (Note) *(paudio->array_start + paudio->offset);
	
	//Display LED for which note we are playing
	switch( current_note )
	{
		case A: bits = 1; break;
		case B: bits = 2; break;
		case C: bits = 4; break;
		case D: bits = 8; break;
		case E: bits = 16; break;
		case F: bits = 32; break;
		case G: bits = 64; break;
		default: bits = 0; break;
	}
	LED_set_enabled( bits );
}

/** This function changes the audio wave output type **/
void SOUND_next_wave_mode()
{
	switch( wave_mode )
	{
		case SQUARE: wave_mode = TRIANGLE; break;
		case TRIANGLE: wave_mode = SAWTOOTH; break;
		case SAWTOOTH: wave_mode = SQUARE; break;
	}
}

/** This function gets the next audio sample **/
short SOUND_get_next_sample()
{
	static short buffer;
	//Do we have something to play?
	if( current_note == SILENCE || current_note == EOT )
	{
		return wave_mode == TRIANGLE ? buffer : 0;
	}

	if( wave_mode == TRIANGLE ) buffer = triangle_wave();
	if( wave_mode == SQUARE   ) buffer = square_wave();
	if( wave_mode == SAWTOOTH ) buffer = sawtooth_wave();

	//No sound
	return buffer;
}

/** This function generates an audio sample for a square wave **/
short square_wave()
{	
	static bool rising = false;

	//Play square wave
	if( freq_clock++ >= current_note )
	{
		freq_clock = 0;
		rising = !rising;
	}

	return rising ? amplitude : -amplitude;
}

/** This function generates an audio sample for a triangle wave **/
short triangle_wave()
{
	static bool rising = true;
	int sound;
	
	if (rising)
	{
		freq_clock += 2;
		if (freq_clock >= current_note)
		{
			rising = false;
			freq_clock = current_note;
		}
	}
	else
	{
		freq_clock -= 2;
		if (freq_clock <= -current_note)
		{
			rising = true;
			freq_clock = -current_note;
		}
	}

	sound = ((freq_clock * amplitude) / current_note)<<2;

	//clip value to a short
	if( sound >= SHRT_MAX ) sound = SHRT_MAX-1;
	if( sound <= SHRT_MIN ) sound = SHRT_MIN+1;

	return sound;
}

/** Generates a single sample of sawtooth audio **/
short sawtooth_wave()
{
	if (freq_clock++ > current_note) freq_clock = -current_note;
	return (freq_clock * amplitude) / current_note;
}

