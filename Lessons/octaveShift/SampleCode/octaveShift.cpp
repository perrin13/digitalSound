/**
 * octaveShift.cpp
 * John Brock
 * Date Created: 17 Jul 2008
 * Date Updated: 25 Jul 2008
 * Purpose: To shift a sound file by a user designated octtave amount 
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
  fprintf(stderr, "Usage Error: ./shift inputFile outFile octaveShift\n");
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


// Finds the maximum magnitude in an array of doubles
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
// Purpose: To octave shift by affect
// Parameters: sound *orig - pointer to the input file
//             char *pC - string denoting the output file
//             double affect - octave shift
// Returns:    sound * - returns a pointer the shifted file class
//
sound *autoT(sound *orig, char *pC, double affect){
 short int *sI;     // Array of sound data
 int i, j, k, l, m = 0; // Various check variables
 int small = 3000;  // Check variable used ot find the smallest instance 
 double dd;         // General purpose double variable
 double *d;         // Stores double array for the FFT
 double *d1;        // Stores double array for the FFT
 double *complex1;  // Stores double array (non-real) for the FFT
 double *complex;   // Stores double array (non-real) for the FFT
 double big, big2;  // Stores double's used to scale new sound file to the original's amplitude
 FILE *in = orig->getFile(); // File containing users input
 FILE *out;         // File pointer to the output stream
 sound *tune = new sound(*orig, pC); // Calls a pre-recording constructor that will give tune, orig's class attributes

 // Set in file to the data chunk
 fseek(in, 44, SEEK_SET);

 // Gets the out file from tune
 out = tune->getFile(); 

 // Dynamically allocates space for data handlers
 d = (double *) calloc(size,sizeof(double));
 d1 = (double *) malloc(sizeof(double)*size);
 complex = (double *) malloc(sizeof(double)*size);
 complex1 = (double *) malloc(sizeof(double)*size);
 sI = (short int *) malloc(sizeof(short int)*orig->getSize());

 // Reads in first chunk of data
 fread(sI, 2, size, in);  
 for(j = 0; j < size; j++)
  d1[j] = sI[j];
 
 // This for loop and the functions it calls do the calculations necessary to shift an octave
 for(i = 0; i < orig->getSize(); i += size){
  // Prepares some arrays for use
  for(j = 0; j < size; j++){
   complex1[j] = 0;
   complex[j] = 0;
   d[j] = 0;
  } 
  
  small = 3000;
  
  // Finds original maximum
  big2 = max_Double(d1, size);
  
  // Converts to the frequency domain
  FFT(1, ((int)log2(size)), d1, complex1);

  dd = pow(2.0, affect);
  // Shifts affect
  for(j = 0; j < (size/2); j++){
    k = ((int)(dd*j + .5));
    if(k < size/2 && k >= 0){
     d[k] = d1[j];
     complex[k] = complex1[j]; 
    }   
  }
  
  // Returns to time domain
  FFT(-1, ((int)log2(size)), d, complex);
  
  // Scales and writes data into the output file
  big = max_Double(d, size);
  for(j = 0; j < size; j++){
   sI[j] = ((short int)(big2*d[j]/big));
  }
  fwrite(sI, 2, size, out);
  

  // Reads innext chunk of data
  fread(sI, 2, size, in); 
  for(j = 0; j < size; j++)
   d1[j] = sI[j]; 
 }
 

 // Closes output and reopens it to be used as a playable file
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
