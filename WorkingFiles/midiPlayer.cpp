/**
 * midiPlayer.cpp
 * By: John Brock
 * Date Created: 24 June 2008
 * Date Updated: 24 June 2008
 * Purpose: To play midi files
 *          -Goal: Play midi notes seperately
**/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <time.h>

#define RATE 8000

// Function Prototypes
int midiPlay(FILE *midi);
float midiNote(FILE *midi);
void dspPlay(float freq);

int main(int argc, char *argv[]){
 char *buf;
 FILE *midi;
 
 if(argc != 2){
  fprintf(stderr, "ERROR: Usage - ./miplay midiFile\n");
  exit(0);
 }
 
 buf = argv[1];
 if((midi = fopen(buf, "r")) == NULL){
  fprintf(stderr, "ERROR: Unable to open file %s\n", buf);
  exit(0);
 }
 
 midiPlay(midi);
 return 0;
}

// 
//  int midiPlay(FILE *midi);
//  Purpose: To play a midi file
//  Parameters: FILE *midi - opened file to be played
//  Return: int - number of notes played
//
int midiPlay(FILE *midi){
 int i;
 float freq;
  
 freq = midiNote(midi);
 while(freq != 0){
  i++;
  dspPlay(freq);
  freq = midiNote(midi);
 }
 return i;
}


//
// float midiNote(FILE *midi)
// Purpose: To parse a midi note from a file and return it's frequency
// Paremeters: FILE *midi - opened file for midi note
// Return: float - frequency of midi note
//
float midiNote(FILE *midi){
 float f;
 int i;
  
 i = fgetc(midi);
 while(i != 0x90){
  if(i == EOF){
   return 0;
  }
  i = fgetc(midi);
 }
 
 i = fgetc(midi);
 fprintf(stderr, "Note: %0X\n", i);
 f = 8.1758*pow(2, i/12.0);
 return f;
}

//
// void dspPlay(float freq)
// Purpose: To play a frequency for breif period
// Parameters: float freq - freq to be played
// Return: void - no return value
//
void dspPlay(float freq){
 int fd;
 int i;
 unsigned char buffer[RATE + 1];
 double samp[RATE];
 
 for(i = 0; i < RATE; i++){
  samp[i] = sin(freq*(2.0*M_PI/RATE)*i);
 }

 for(i = 0; i < RATE; i++){
  buffer[i] = ((int)(((samp[i] + 2)/3)*255));
 }

 fd = open("/dev/dsp", O_RDWR);
 time_t start = time(NULL);
 while(start+1>time(NULL)){
  write(fd, buffer, sizeof(buffer));
 }
 
 close(fd);
}

