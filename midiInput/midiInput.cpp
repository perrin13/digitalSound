/**
 * midiInput.cpp
 * By: John Brock
 * Date Created: 1 July 2008
 * Date Updated: 1 July 2008
 * Purpose: To interpret input from a midi control surface
 *             -Print midi commands to stdout
 *             -Play midi notes entered
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
typedef void (*fptr)(int signum);

void endLoop(int signum);
void dspPlay(unsigned char *midi);

FILE *fd;
int fo;
#define RATE 8000

int main(){
 unsigned char c[4];
 
 signal(SIGINT, (fptr)endLoop);
 if((fd = fopen("/dev/snd/midiC1D0", "r")) == NULL){
  exit(1);
 }
 
 fo = open("/dev/dsp", O_RDWR);
 
 while(true){
  fread(c, 1, 1, fd);
  if(((int)c[0]) & 0x80){
   fread(&c[1], 1, 1, fd);
   fprintf(stdout, "Command: %0X\n Data1: %0X\n", c[0], c[1]);
   if((((int)c[0])&0xC0) != 0xC0 && (((int)c[0])&0xD0) != 0XD0){
    fread(&c[2], 1, 1, fd);
    fprintf(stdout, " Data 2: %0X\n", c[2]);
   } 
   if((((int)c[0])&0xF0) == 0x90)
    dspPlay(c);  
  }
 }
 
 fclose(fd);
 close(fo);
 return 0;
}

void endLoop(int signum){
 fclose(fd);
 close(fo);
 exit(0);
}

//
// void dspPlay(unsigned char *midi)
// Purpose: To play a midi note for brief period
// Parameters: char *midi - pointer to an array of midi data to be played
// Return: void - no return value
//
void dspPlay(unsigned char *midi){
 int i;
 double freq;
 unsigned char buffer[RATE + 1];
 double samp[RATE];
 
 if(midi[2] == 0)
  return;
 
 freq  = 8.1758*pow(2, ((int)midi[1])/12.0);    
 for(i = 0; i < RATE; i++){
  samp[i] = sin(freq*(2.0*M_PI/RATE)*i);
 }

 for(i = 0; i < RATE; i++){
  buffer[i] = ((int)(((samp[i] + 1)/2)*255));
 }

 write(fo, buffer, sizeof(buffer));
}
