/**
 * midiSample2.cpp
 * By: John Brock
 * Date Created: 3 July 2008
 * Date Updated: 3 July 2008
 * Purpose: To make a midi sample file
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
#define RATE 44100
#define LENGTH 5

typedef void (*fptr)(int signum);

FILE *fo;
int fd;

void endLoop(int signum);

int main(int argc, char *argv[]){
 int i;
 unsigned char m;
 double samp[RATE*LENGTH];
 for(i = 0; i <RATE*LENGTH; i++)
  samp[i] = 0;
 int j, k;
 double freq;
 unsigned char buffer[RATE*LENGTH];
 char *in;

 if(argc != 2){
  fprintf(stderr, "ERROR: Improper usage (./mSamp2 sampleFile)\n");
  exit(1);
 }

 in = argv[1];
 if((fo = fopen(in, "w")) == NULL){
  fprintf(stderr, "ERROR: Unable to open file %s\n", in);
  exit(1);
 }
  
 if((fd = open("/dev/dsp", O_RDWR)) < 0){
  fprintf(stderr, "ERROR: Unable to open DSP\n");
  exit(1);
 }
  
 j = RATE;
 k = ioctl(fd, SOUND_PCM_WRITE_RATE, &j);
 if(k == -1){
  fprintf(stderr, "ERROR: SOUND_PCM_WRITE_RATE failed\n");
  exit(1);
 }

 for(m = 0; m < 127; m++){
  fprintf(stderr, "Midi: %d\n", m);
  fprintf(fo, "\n%c:\n", m);
  
  freq = 8.1758*pow(2, m/12.0);
  for(i = 0; i < RATE*LENGTH; i++){
   samp[i] += sin(freq*(2.0*M_PI/RATE)*i);
  }
  for(i = 0; i < RATE*LENGTH; i++){
   buffer[i] = ((int)(((samp[i] + 1.0)/2)*255/m));
  }
  
  fwrite(buffer, RATE*LENGTH, 1, fo);
  write(fd, buffer, RATE*LENGTH);
 }
 
 close(fd);
 fclose(fo);
 
 return 0;
}





// Safe Close
void endLoop(int signum){
 close(fd);
 fclose(fo);
 exit(0);
}

