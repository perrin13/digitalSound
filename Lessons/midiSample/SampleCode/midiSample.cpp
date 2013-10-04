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
#define RATE 8000  // Defines sample rate
#define LENGTH 5   // Defines play time in seconds

typedef void (*fptr)(int signum);  // File pointer definition

FILE *fd;   // Globally declared file pointer to allow for instantaneous closing

void endLoop(int signum); // Function used to allow user to send interrupt to harmlessly end the program

int main(int argc, char *argv[]){
 int i, g;   // Various test variables
 unsigned char m;  // General purpose variable used mostly for finding the midi number from the user's note input
 double samp[RATE*LENGTH];  // double array used for sampled data
 for(i = 0; i < RATE*LENGTH; i++)
   samp[i] = 0;
 int j, k; // test variables
 string midi; // Contains the midi string
 double freq; // Contains the frequency obtained for each note
 unsigned char *buffer; //Buffer that contains the data to be fed into the dsp
 signal(SIGINT, (fptr)endLoop);

 if(argc == 1){
  fprintf(stderr, "ERROR: Improper usage (./mSamp 1-4)\n");
  exit(1);
 } 

 
 j = atoi(argv[1]);
 buffer = (unsigned char *) calloc(RATE*LENGTH + 1, 1);
 
 // This for loop finds the samples needed to play the notes input
 for(k = 0; k < j; k++){
  
  // Get the user's note
  fprintf(stderr, "Please enter a note: ");
  cin >> midi;
  
  // Find the note's midi number
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
  
  // Convert the midi number to a frequency
  freq = 8.1758*pow(2, m/12.0);
  fprintf(stderr, "Midi: %lf\n", freq);

  // Find the samples
  for(i = 0; i < RATE*LENGTH; i++){
   samp[i] += sin(freq*(2.0*M_PI/(RATE))*i);
   fprintf(stderr, "samp[i] = %lf\n", samp[i]);
  }
 }
  
 // Scale and write samples into a character buffer
 for(i = 0; i < RATE*LENGTH; i++){
  buffer[i] = ((int)(((samp[i]/j + 1.0)/2)*255));
 }
 
 fd = fopen("/dev/dsp", "w");
 // Write samples to the dsp (play them)
 fwrite(buffer, RATE*LENGTH, 1, fd);
 fclose(fd);
 return 0;
}


void endLoop(int signum){
 fclose(fd);
 exit(0);
}
