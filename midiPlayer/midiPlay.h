/**
 * midiPlay.h
 * By: John Brock
 * Date Created: 25 June 2008
 * Date Updated: 25 June 2008
 * Purpose: Contains basic specifications for a midiPlayer
**/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <time.h>

#ifndef M_PLAY
#define M_PLAY

// Variable Adjustment:
#define RATE 44100  // Sample rate
#define CHANNELS 1 // 1 = mono 2 = stereo
float length = .25;      // time division
unsigned char *buf;
void endLoop(int signum);
int fd;

unsigned char *shm;
int shmsz;

// MIDI file properties
char format;
short int tracks;
short int timeD;

// Function prototypes
void headerParse(FILE *midi);
void dspChange(FILE *midi);

#endif
