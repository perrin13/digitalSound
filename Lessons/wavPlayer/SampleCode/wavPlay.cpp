/**
 * cSound.cpp
 * John Brock
 * Date: 9 Jul 2008
 * Updated: 9 Jul 2008
 * Purpose: To play sound files 
 *            - Type 1 wave files
**/
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

 // Notice the required order for playing a file.  Yes it is nto necessary but it allows more flexibility
 // If you dont like having all of these commands, perhaps you should add another function that does them all
 sound noob(input);
 noob.soundParse();
 noob.soundPlay();

 // This program records if and only if the user inputs a file to record to as their second argument
 if(argv > 2){
  pC = argc[2];
  sound nub;
  
  nub.soundRecord(noob, pC); 
  nub.soundPlay();
 }
 
 fclose(input);
 return 0;
}
