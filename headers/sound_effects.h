//This header file contains all the audio data for the various sound effects
//The format works like this: [ note, note_duration ] where X means silence (no note)
//A sound track is always played from beginning to end

#ifndef SOUND_EFFECTS_INCLUDED
#define SOUND_EFFECTS_INCLUDED
int sample_song[] = { 
 X, 500,
 C, 2400,
 X, 500,
 D, 2400,
 X, 500,
 E, 2400,
 X, 500,
 F, 2400,
 X, 500,
 G, 4800,
 X, 500,
 G, 4800,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 A, 2400,
 X, 500,
 G, 6000,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 F, 2400,
 X, 500,
 E, 4800,
 X, 500,
 E, 4800,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 D, 2400,
 X, 500,
 C, 10000
 };


int paddle_fail[] = { 

F, 1000,
E, 1000,
D, 1000,
C, 2000,

};

int paddle_left[] = { 

 C, 100,
 D, 100,
 E, 100,
 F, 100,
 G, 100,
 A, 100,
 B, 100

 }; 


int paddle_right[] = { 
 B, 100,
 A, 100,
 G, 100,
 F, 100,
 E, 100,
 D, 100,
 C, 100
 };

int victory_song[] = { 
C, 300,
D, 300,
E, 300,
F, 300,
G, 300,
F, 300,
E, 300,
D, 300,
C, 300,

C, 300,
D, 300,
E, 300,
F, 300,
G, 300,
F, 300,
E, 300,
D, 300,
C, 300,

C, 300,
D, 300,
E, 300,
F, 300,
G, 300,
F, 300,
E, 300,
D, 300,
C, 300,

C, 500,
B, 500,
C, 500,
B, 500,
A, 1500
 }; 

int pong_song[] = { 
C, 1200,
D, 1200,
X, 750,
D, 1200,
X, 750,
D, 1200,
F, 1500,
X, 750,
G, 1500,
X, 750,
E, 1500,
X, 750,
D, 1500,
X, 750,
F, 1500,
X, 750,
E, 2250,

C, 1200,
D, 1200,
X, 750,
D, 1200,
X, 750,
D, 1200,
F, 1500,
X, 750,
G, 1500,
X, 750,
E, 1500,
X, 750,
D, 1500,
X, 750,
F, 1500,
X, 750,
G, 3250, 

X, 750,
G, 1500,
X, 750,
F, 1500,
X, 750,
E, 1500,
X, 750,
D, 1500,
X, 750,
F, 1500,
X, 750,

G, 1200,
F, 1200,
X, 750,
F, 1200,
X, 750,
E, 1200,
D, 1500,
X, 750,
D, 1500,
X, 750,
E, 1500,
X, 750,
D, 1500,
X, 750,
E, 1500,
X, 750,
C, 2250, 

 };

#endif
