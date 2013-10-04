/**
 * midiSample.cpp
 * John Brock
 * 26 June 2008
 * Purpose: To create samples for midi states
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
#include <iostream>
using namespace std;
#define RATE 8000
#define LENGTH 5

typedef void (*fptr)(int signum);

FILE *fd;

void endLoop(int signum);

int main(int argc, char *argv[]){
 int i, g;
 unsigned char m;
 double samp[RATE*LENGTH];
 for(i = 0; i < RATE*LENGTH; i++)
   samp[i] = 0;
 int j, k;
 string midi;
 double freq;
 unsigned char *buffer;

 if(argc == 1){
  fprintf(stderr, "ERROR: Improper usage (./mSamp 1-4)\n");
  exit(1);
 } 

 
 j = atoi(argv[1]);
 buffer = (unsigned char *) calloc(RATE*LENGTH + 1, 1);

 for(k = 0; k < j; k++){
  fprintf(stderr, "Please enter a note: ");
  cin >> midi;
  m = midi[0] - 'A';
 
  switch(m){
   case 0: m = 9; break;
   case 1: m = 11; break;
   case 2: m = 0; break;
   case 3: m = 2; break;
   case 4: break;
   case 5: break;
   case 6: m = 7; break;
   default: fprintf(stderr, "ERROR: improper note format"); exit(1); break;
  }
 
  m = m + 12*(midi[1] - 3 - '0') + 0x3C;
  if(midi[2] == '#')
   m++;
  if(midi[2] == 'b')
   m--;
 
  fprintf(stderr, "Midi: %d\n", m);
  freq = 8.1758*pow(2, m/12.0);
  fprintf(stderr, "Midi: %lf\n", freq);
  for(i = 0; i < RATE*LENGTH; i++){
   samp[i] += sin(freq*(2.0*M_PI/(RATE))*i);
   fprintf(stderr, "samp[i] = %lf\n", samp[i]);
  }
 }
 for(i = 0; i < RATE*LENGTH; i++){
  buffer[i] = ((int)(((samp[i]/j + 1.0)/2)*255));
 }
 fd = fopen("/dev/dsp", "w");
 fwrite(buffer, RATE*LENGTH, 1, fd);
 fclose(fd);
 return 0;
}

void endLoop(int signum){
 fclose(fd);
 exit(0);
}
