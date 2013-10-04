/**
 * freqSep.cpp
 * John Brock
 * Date Created: 17 Jul 2008
 * Date Updated: 21 Jul 2008
 * Purpose: To attempt to autotune
**/
#include "sound.h"
#include "fft.h"
#define size 8192
// Function Prototypes
sound *autoT(sound *orig, char *pC);
int frequency[] = {65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 350, 370, 415, 440,  466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2784, 2960, 3136, 3322, 3520, 3729, 3951};
  
int main(int argc, char *argv[]){
 FILE *input;
 char *pC;

 if(argc < 3){
  fprintf(stderr, "Usage Error: ./aTune inputFile outFile\n");
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
 
 tune = autoT(orig, pC);
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

void freqBase(const double freq, int *baseFreq){
 int i;
  
 baseFreq = (int *) calloc(72, sizeof(int));
 for(i = 0; i < 72; i++){
  baseFreq[i] = ((int) (frequency[i]/freq));
  fprintf(stderr, "baseFreq[%d] = %d\n", i, baseFreq[i]);
 }

}

//
// void autoT(sound *orig, sound *tune)
// Purpose: To create a tuning affect for each band
// Parameters: sound *orig - pointer to the input file
//             sound *tune - pointer to the output file
//
sound *autoT(sound *orig, char *pC){
 short int *sI;
 int i, j, k, l, m = 0;;
 int small = 3000;
 const double freq = orig->getRate()/(size/2);
 int files = size/2;
 int *baseFreq;
 baseFreq = (int *) calloc(72, sizeof(int));
 freqBase(freq, baseFreq);
 double *d;
 double *d1;
 double *complex1;
 double *complex;
 double big, big2;
 char *buf;
 unsigned char *buff;
 FILE *in = orig->getFile();
 FILE *out;
 sound *tune = new sound(*orig, pC);
 sound *sep = new sound[files];
 buff = (unsigned char *) malloc(45);
 
 for(i = 0; i < files; i++){
  fseek(in, 0, SEEK_SET);
  sprintf(buf, "%c%c%c%c%c%d%c", pC[0], pC[1], pC[2], pC[3], pC[4], i, 0);
  if((out = fopen(buf, "w"))==NULL){
   fprintf(stderr, "ERROR: Failed to open file %s\n", buf);
   exit(1);
  }
  
  
  fread(buff, 1, 44, in);
  fwrite(buff, 1, 44, in);
  fclose(out);
  out = fopen(buf, "a");
  
  sep[i].setInput(out);
 } 

 out = tune->getFile(); 
 fprintf(stderr, "autoT\n");
 d = (double *) calloc(size,sizeof(double));
 d1 = (double *) malloc(sizeof(double)*size);
 complex = (double *) malloc(sizeof(double)*size);
 complex1 = (double *) malloc(sizeof(double)*size);
 sI = (short int *) malloc(sizeof(short int)*orig->getSize());

 fread(sI, 2, size, in); 
 //fwrite(sI, 2, orig->getSize()/2, out);
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

  for(k = 0; k < size/2; k++){
   for(l = 0; l < size/2; l++){
    d[l] = 0;
    complex[l] = 0;
    d[k] = d1[k];
    complex[k] = complex1[k];
   }
   FFT(-1, ((int)log2(size)), d, complex);
   
   out = sep[i].getFile();

   big = max_Double(d, size);
   for(j = 0; j < size; j++){
    sI[j] = ((short int)(/*big2*/32767*d[j]/big));
   }
   fwrite(sI, 2, size, out);
  }
  
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
