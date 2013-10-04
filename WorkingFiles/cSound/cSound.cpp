/**
 * cSound.cpp
 * John Brock
 * Date: 9 Jul 2008
 * Updated: 9 Jul 2008
 * Purpose: To play sound files 
 *            - Type 1 wave files
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
#include "sound.h"

int main(int argv, char *argc[]){
 char *pC;
 FILE *input;
  
 if(argv < 2){
  fprintf(stderr, "ERROR: Improper input (./cPlay soundFile)\n");
  exit(1);
 }

 pC = argc[1];
 if((input = fopen(pC, "r")) == NULL){
  fprintf(stderr, "ERROR: Failed to open file %s\n", pC);
  exit(1);
 }

 sound noob(input);
 noob.soundParse();

 noob.soundPlay();
 if(argv > 2){
  pC = argc[2];
  sound nub(pC);;
  
  nub.soundRecord(noob); 
  nub.soundPlay();
 }
 fclose(input);
 return 0;
}
