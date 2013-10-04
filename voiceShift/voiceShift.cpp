/**
 * autoTune.cpp
 * John Brock
 * Date Created: 17 Jul 2008
 * Date Updated: 23 Jul 2008
 * Purpose: To attempt to autotune
**/
#include "sound.h"
#include "fft.h"
#define margin 0.03
#define size 8192
// Function Prototypes
sound *autoT(sound *orig, char *pC, double in);
  
int main(int argc, char *argv[]){
 FILE *input;
 char *pC;
 double in;
 int i;

 if(argc < 4){
  fprintf(stderr, "Usage Error: ./aTune inputFile outFile octaveShift\n");
  exit(1);
 }
 
 pC = argv[1];
 if((input = fopen(pC, "r")) == NULL){
  fprintf(stderr, "ERROR: Failed to open input file: %s\n", pC);
  exit(1);
 } 
 
 sound *orig = new sound(input);
 orig->soundParse();
 
 pC = argv[2];
 sound *tune;
 
 i = strlen(argv[3]);
 in = strtod(argv[3], NULL);
 
 tune = autoT(orig, pC, in);
}


// Finds the maximum value in an array of doubles
double max_Double(double *d, int length){
 int i;
 double big;
 
 if(d[0] > 0)
  big = d[0];
 if(d[0] < 0)
  big = -d[0];
 for(i = 0; i < length; i++){
  if(d[i] > big)
   big = d[i];
  if(-d[i] > big) 
   big = -d[i];
 }
 return big;
}

//
// void autoT(sound *orig, sound *tune)
// Purpose: To create a tuning affect for each band
// Parameters: sound *orig - pointer to the input file
//             sound *tune - pointer to the output file
//
sound *autoT(sound *orig, char *pC, double affect){
 short int *sI;
 int i, j, k, l, m = 0;
 int small = 3000;
 double dd;
 double *d;
 double *d1;
 double *complex1;
 double *complex;
 double big, big2;
 FILE *in = orig->getFile();
 FILE *out;
 sound *tune = new sound(*orig, pC);
 fseek(in, 44, SEEK_SET);

 out = tune->getFile(); 
 fprintf(stderr, "autoT\n");
 d = (double *) calloc(size,sizeof(double));
 d1 = (double *) malloc(sizeof(double)*size);
 complex = (double *) malloc(sizeof(double)*size);
 complex1 = (double *) malloc(sizeof(double)*size);
 sI = (short int *) malloc(sizeof(short int)*orig->getSize());

 fread(sI, 2, size, in); 
 
 for(j = 0; j < size; j++)
  d1[j] = sI[j];
 
 for(i = 0; i < orig->getSize(); i += size){
  for(j = 0; j < size; j++){
   complex1[j] = 0;
   complex[j] = 0;
   d[j] = 0;
  } 
  small = 3000;

  big2 = max_Double(d1, size);
  FFT(1, ((int)log2(size)), d1, complex1);

  dd = affect;
  for(j = 0; j < (size/2); j++){
    k = ((int)(pow(2.0, dd)*j + .5));
    if(k < size/2 && k >= 0){
     d[k] = d1[j];
     complex[k] = complex1[j]; 
    }   
  }
  
  FFT(-1, ((int)log2(size)), d, complex);
  
  big = max_Double(d, size);
  for(j = 0; j < size; j++){
   sI[j] = ((short int)(big2*d[j]/big));
  }
  fwrite(sI, 2, size, out);
  
  fread(sI, 2, size, in); 
  for(j = 0; j < size; j++)
   d1[j] = sI[j]; 
 }
 
 fclose(out);
 if((out = fopen(pC, "r")) == NULL){
  fprintf(stderr, "Unable to open %s\n", pC);
  exit(1);
 }
  
 
 tune->setInput(out);
 tune->soundParse();
 tune->soundPlay();
 return tune;
}
