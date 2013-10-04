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

// Finds the index of the maximum of a list of complex doubles
int max_Complex(double *d, double *c, int length){
 int i, k, l, *j;
 j = (int *) calloc(5, sizeof(int));
 double *big;
 big = (double *) calloc(5, sizeof(double));
 double magnitude;
 double dd;
 double cc;
 
 for(i = 10; i < length; i++){
  dd = d[i];
  cc = c[i];
  magnitude = sqrt(pow(dd, 2) + pow(cc, 2));
  // magnitude = d[i]*d[i];
  if(magnitude > big[0]){
   // fprintf(stderr, "Magnitude: %lf\n", magnitude);
   // fprintf(stderr, "d[%d] = %lf\n", i, d[i]);
   big[4] = big[3];
   big[3] = big[2];
   big[2] = big[1];
   big[1] = big[0];
   big[0]  = magnitude;
   
   j[4] = j[3];
   j[3] = j[2];
   j[2] = j[1];
   j[1] = j[0];
   j[0] = i;
  } else if(magnitude > big[1]){ 
  if((j[0] > margin*i + i + 1 || j[0] < i - margin*i - 1)){
   big[4] = big[3];
   big[3] = big[2];
   big[2] = big[1];
   big[1] = magnitude;
   
   j[4] = j[3];
   j[3] = j[2];
   j[2] = j[1];
   j[1] = i;
  }  
  } else if(magnitude > big[2]){
   if((j[0] > margin*i + i + 1 || j[0] < i - margin*i - 1)&&(j[1] > margin*i + i + 1 || j[1] < i - margin*i - 1)){
   big[4] = big[3];
   big[3] = big[2];
   big[2] = magnitude;
   
   j[4] = j[3];
   j[3] = j[2];
   j[2] = i;  
  }
  } else if(magnitude > big[3]){
  if((j[0] > margin*i + i + 1 || j[0] < i - margin*i - 1)&&(j[1] > margin*i + i + 1 || j[1] < i - margin*i - 1)&&(j[2] > margin*i + i + 1 || j[2] < i - margin*i - 1)){ 
   big[4] = big[3];
   big[3] = magnitude;
   
   j[4] = j[3];
   j[3] = i;
  }
  } else if(magnitude > big[4]){
  if((j[0] > margin*i + i + 1 || j[0] < i - margin*i - 1)&&(j[1] > margin*i + i + 1 || j[1] < i - margin*i - 1)&&(j[2] > margin*i + i + 1 || j[2] < i - margin*i - 1)&&(j[3] > margin*i + i + 1 || j[3] < i - margin*i - 1)){
   big[4]  = magnitude;
   j[4] = i;
  }
  }
 }
 
 for(i = 0; i < 4; i++){
  for(k = 0; k < 4; k++){
   if(j[i] > j[k]){
    l = j[i];
    dd = big[i];
    j[i] = j[k];
    big[i] = big[k];
    j[k] = l;
    big[k] = dd;
   }
  }
 }
 
 k = j[0];
 for(i = 4; i > 0 && k == j[0]; i--){
   //fprintf(stderr, "j[%d] = %d\n", i, j[i]);
   if((j[0]/2 > j[i] - j[i]*margin && j[0]/2 < j[i] + j[i]*margin)||(j[0]/3 > j[i] - j[i]*margin && j[0]/3 < j[i] + j[i]*margin)||(j[0]/4 > j[i] - j[i]*margin && j[0]/4 < j[i] + j[i]*margin)||(j[0]/5 > j[i] - j[i]*margin && j[0]/5 < j[i] + j[i]*margin)||(j[0]/6 > j[i] - j[i]*margin && j[0]/6 < j[i] + j[i]*margin)){
    k = j[i];
   } 
 }
 
 // cerr << k;
 return k;
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
 int i, j, k, l, m = 0;
 int small = 3000;
 const double freq = orig->getRate()/(size);
 int *baseFreq;
 baseFreq = (int *) calloc(72, sizeof(int));
 freqBase(freq, baseFreq);
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
  m = max_Complex(&d1[1], complex1, size/2);
  // fprintf(stderr, "m: %d", m);
  k = (m)*freq;
  // fprintf(stderr, "k: %d\n", k);
  for(j = 0; j < 72; j++){
   if(abs(frequency[j] - k) < small){
    small = abs(frequency[j] - k);
    // fprintf(stderr, "Small: %d\n", small);
    l = j;
   }
  }
  // fprintf(stderr, "k = %d\n",k);
  l = l - m;
  
  for(j = 0; j < (size/2); j++){
   /*k = j*freq + freq/2;
   small = 3000;
   for(l = 0; l < 72; l++){
    if(frequency[l] < abs(k)){
     m = l;
    }
   }*/
   d[j] = d1[j];
   complex[j] = complex1[j];
   if(j < m - 16 || j > m +16){
    d[j] = 0;
    complex[j] = 0;
   }
    /*if(j+l < size/2 && j+l >= 0){
     d[j + l] = d1[j];
     complex[j + l] = complex1[j]; 
    }   */
  }
  
  FFT(-1, ((int)log2(size)), d, complex);
  
  big = max_Double(d, size);
  for(j = 0; j < size; j++){
   sI[j] = ((short int)(/*big2*/32627*d[j]/big));
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
 // tune->soundPlay();
 return tune;
}
