/**
 * midiPlayer.cpp
 * By: John Brock
 * Date Created: 24 June 2008
 * Date Updated: 25 June 2008
 * Purpose: To play midi files
 *          -Goal: Play midi notes seperately - check
 *          -Goal: Play notes together in chords - 
**/
#include "midiPlay.h"
typedef void (*fptr)(int signum);

int main(int argc, char *argv[]){
 char *buf;
 FILE *midi;
 signal(SIGINT, (fptr)endLoop);
 
 if(argc != 2){
  fprintf(stderr, "ERROR: Usage - ./mPlay midiFile\n");
  exit(0);
 }
 
 buf = argv[1];
 if((midi = fopen(buf, "r")) == NULL){
  fprintf(stderr, "ERROR: Unable to open file %s\n", buf);
  exit(0);
 }
  
 headerParse(midi);

 shm = (unsigned char *) calloc(shmsz, 1);

 dspChange(midi);

 fclose(midi);
 return 0;
}

//
// void dspChange(FILE *midi)
// Purpose: Changes buffer to be played
// Parameters: FILE *midi - midi source file
//
void dspChange(FILE *midi){
 int freq;
 double *samp;
 samp = (double *) calloc(shmsz, sizeof(double));
 unsigned char *test;
 test = (unsigned char *) calloc(shmsz, sizeof(double));
 int i, k, j = 1;
 int d1, d2;

 fd = open("/dev/dsp", O_RDWR);
 i = RATE;
 k = ioctl(fd, SOUND_PCM_WRITE_RATE, &i);
 if(k == -1){
  fprintf(stderr, "SOUND_PCM_WRITE_RATE ioctl failed\n");
  exit(1);
 }

 while((k =  fgetc(midi)) != EOF && k  != 0x90);
 if(k == EOF){
  fprintf(stderr, "ERROR: Premature end of file");
  exit(1);
 }
 
 fprintf(stderr, "%d\n", k);
 while(j){
  if(k&0x80){
   d1 = fgetc(midi);
   if(k != 0xC0 && k != 0xD0){
    d2 = fgetc(midi);
   }
   if(k == 0xFF && d1 == 0x2F)
    j = 0; 
   if(k == 0x90){
    freq = 8.1758*pow(2, d1/12.0);
    if(d2 != 0){
     for(i = 0; i < (shmsz); i++){
      samp[i] += sin(freq*(2.0*M_PI/RATE)*i);
     }
     for(i = 0; i < shmsz; i++){
      test[i] = ((unsigned char)(((samp[i]*.25+1.0)/2*255)));
     }
    }else{
     for(i = 0; i < (shmsz); i++){
      samp[i] -= sin(freq*(2.0*M_PI/RATE)*i);
     }
     for(i = 0; i < shmsz; i++){
      test[i] = ((unsigned char)(((samp[i]*.25+1.0)/2*255)));
     }
    }
   }
  } else{
   write(fd, test, shmsz*length*k);
  }
  k = fgetc(midi);
 }
 close(fd);
}

//
// void headerParse(FILE *midi)
// Purpose: To parse info from the midi header file
// Parameters: FILE *midi - midi file to parse
// Return: void - no return value(alters global variables)
//
void headerParse(FILE *midi){
 char c[5];
 short int ticks;
 short int frames;
 // ensure that file is a midi file
 fread(c, 1, 4, midi);
 c[5] = '\0';
 if(strcmp(c, "MThd") != 0){
  fprintf(stderr, "::%s", c);
  fprintf(stderr, "ERROR: Opened file is not of midi format\n");
  exit(0);
 }

 // read in format type
 fread(c, 1, 4, midi);
 fread(c, 1, 1, midi);
 fread(c, 1, 1, midi);
 format = c[0];
 
 // read in number of tracks
 fread(c, 1, 2, midi);
 tracks = c[0]*16 + c[1];

 // Read in time division
 fread(c, 1, 2, midi);
 timeD = c[0]*16 + c[1];

 // Parse time division to find length - assume tempo of 120 beats per sec
 if((timeD&0x8000)){
  ticks = 0x00FF&timeD;
  frames = (0x7F00&timeD)/16;
  length = 1.0/(ticks*frames);
 } else{
  ticks = timeD;
  length = 1.0/(ticks*2);
 }
 
 shmsz = ((int) (RATE*CHANNELS));
 fprintf(stderr, "Length: %lf\n", length);
}

void endLoop(int signum){
 close(fd);
 exit(0);
}

