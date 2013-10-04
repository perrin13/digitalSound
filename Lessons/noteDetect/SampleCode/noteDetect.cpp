/**
 * noteDetect.cpp
 * John Brock
 * Date Created: 17 Jul 2008
 * Date Updated: 23 Jul 2008
 * Purpose: Returns note recorded into it
**/
#include <math.h>
#include "sound.h"
#include "fft.h"
#define margin .03 // define the acceptable margin of error

#define size 16384 // define the number of samples to be used for each FFT

// Function Prototypes
void autoT(sound *orig);

// Contains an array of frequencies of pure tones
int frequency[] = {65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 350, 370, 392, 415, 440,  466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2784, 2960, 3136, 3322, 3520, 3729, 3951};

// Indexed the same as frequency for easy identification 
string freque[] = {"C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2", "C3", "C3#", "D3", "D3#", "E3", "F3", "F3#", "G3", "G3#", "A3", "A3#", "B3","C4", "C4#", "D4", "D4#", "E4", "F4", "F4#", "G4", "G4#", "A4", "A4#", "B4","C5", "C5#", "D5", "D5#", "E5", "F5", "F5#", "G5", "G5#", "A5", "A5#", "B5","C6", "C6#", "D6", "D6#", "E6", "F6", "F6#", "G6", "G6#", "A6", "A6#", "B6","C7", "C7#", "D7", "D7#", "E7", "F7", "F7#", "G7", "G7#", "A7", "A7#", "B7"};
  
int main(int argc, char *argv[]){
 FILE *input;  // The users file
 char *pC;     // A general purpose character pointer


 // Ensures that enough arguments are present for the program to run
 if(argc < 2){
  fprintf(stderr, "Usage Error: ./note inputFile \n");
  exit(1);
 }
 
 pC = argv[1];
 

 // Open's user's file
 if((input = fopen(pC, "r")) == NULL){
  fprintf(stderr, "ERROR: unable to open %s\n", pC);
  exit(1);
 }
 sound *orig = new sound(input);
 orig->soundParse();
 
 // Runs note detect
 autoT(orig);
}


// 
// double max_Double(double *d, int length)
// Purpose: To return the double with the largest magnitude in an array
// Parameters: double *d - An array of doubles
//             int length - length of the array
// Return:     double - the double with the largest magnitude in the array
//
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
// int max_Complex(double *d, double *c, int length)
// Purpose: To find the index of the fundamental frequency
// Parameters: double *d - pointer to an array of the real parts of the frequency bands
//             double *c - pointer to an array of the non-real parts of the frequency bands
//             int length - the number of frequency bands
// Return:     int - the index of the fundamental frequency
// 
int max_Complex(double *d, double *c, int length){
 int i, k, l;  // Various check variables
 int  *j;      // Pointer to an array of the indexes of the 5 highest relative maximums
 j = (int *) calloc(5, sizeof(int));

 double *big;  // Pointer to an array of the magnitudes of the 5 highest relative maximums
 big = (double *) calloc(5, sizeof(double));
 double magnitude; // check double that holds the most recently calculated magnitude 
 double dd;        // Multipurpose double
 double cc;        // Multipurpose double
 

 // This for loop finds the 5 largest magnitudes whose indexes are at least 1 + 5% apart
 for(i = 10; i < length; i++){
  dd = d[i];
  cc = c[i];
  magnitude = sqrt(pow(dd, 2) + pow(cc, 2));
 
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

 // This for loop sorts the calculated magnitudes from largest index to smallest
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
 // This for loop finds the lowest indexed magnitude that is an integer divisor (w/in margin%) of the highest indexed magnitude
 // This method uses the fact that the lowest harmonic is actaully the fundamental frequency
 for(i = 4; i > 0 && k == j[0]; i--){
   fprintf(stderr, "j[%d] = %d\n", i, j[i]);
   if((j[0]/2 > j[i] - j[i]*margin && j[0]/2 < j[i] + j[i]*margin)||(j[0]/3 > j[i] - j[i]*margin && j[0]/3 < j[i] + j[i]*margin)||(j[0]/4 > j[i] - j[i]*margin && j[0]/4 < j[i] + j[i]*margin)||(j[0]/5 > j[i] - j[i]*margin && j[0]/5 < j[i] + j[i]*margin)||(j[0]/6 > j[i] - j[i]*margin && j[0]/6 < j[i] + j[i]*margin)) {
    k = j[i];
   } 
 }

 
 // cerr << k;
 return k;
}

//
// void freqBase(const double freq, int *baseFreq)
// Purpose: To find the frequency band index of the pure notes
// Parameters: const double freq - frequency band width
//             int *baseFreq - Array to contain the indexes
// Returns:    void - the baseFreq array is altered
//
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
// Purpose: To find the notes in the sound file
// Parameters: sound *orig - pointer to the input file
//             sound *tune - pointer to the output file
//
void autoT(sound *orig){
 short int *sI;           // buffer for sound data
 int i, j, k, l, m = 0;  // various check variables
 int small = 3000;        // check variable that is used to find a small number
 const double freq = orig->getRate()/(1.0*size); // Constant that defines the width of frequency bands
 int *baseFreq;           // Array of integers that holds the indexes of the pure note frequencies

 baseFreq = (int *) calloc(72, sizeof(int));  
 freqBase(freq, baseFreq);     // Finds the indexes fo the pure note frequencies
 double *d;        // Pointer to an array of doubles used in FFT calculations
 double *d1;       // Pointer to an array of doubles used in FFT calculations
 double *complex1; // Pointer to an array of doubles (complex part) used in FFT calculations
 double *complex;  // Pointer to an array of doubles (complex part) used in FFT calculations
 double big, big2; // Two doubles used to scale data to the original range
 FILE *in = orig->getFile(); // Gets user's file from the orig sound class
 fseek(in, 44, SEEK_SET);    // Sets location to the data struct

 // fprintf(stderr, "autoT\n");  

 // Dyanmically prepare space for various varible arrays
 d = (double *) calloc(size,sizeof(double));
 d1 = (double *) malloc(sizeof(double)*size);
 complex = (double *) malloc(sizeof(double)*size);
 complex1 = (double *) malloc(sizeof(double)*size);
 sI = (short int *) malloc(sizeof(short int)*orig->getSize());

 // Read in first chunk of data
 fread(sI, 2, size, in); 

 for(j = 0; j < size; j++)
  d1[j] = sI[j];
 
 // This for loop and the functions it calls contains all of the major calculations
 for(i = 0; i < orig->getSize(); i += size){
  // Clears out some arrays for use
  for(j = 0; j < size; j++){
   complex1[j] = 0;
   complex[j] = 0;
   d[j] = 0;
  } 
  small = 3000;
  
  // Finds the original largest data piece
  big2 = max_Double(d1, size);

  // Performs an FFT to convert to the frequency domain 
  FFT(1, ((int)log2(size)), d1, complex1);
 
  //for(j = 0; j < size/2; j++){
  // printf("d1[%d] = %lf\n complex1[%d] = %lf\n", j, d1[j], j, complex1[j]);
  //}
  
  // Finds the index of the fundamental frequency
  m = max_Complex(&d1[1], complex1, size/2);
  // fprintf(stderr, "m: %d", m);
  
  // Determines fundamental frequency
  k = (m)*freq;
  // fprintf(stderr, "k: %d\n", k);
  
  // Determines fundamental frequencies pitch
  for(j = 0; j < 72; j++){
   if(abs(frequency[j] - k) < small){
    small = abs(frequency[j] - k);
    // fprintf(stderr, "Small: %d\n", small);
    l = j;
   }
  }
  // cerr << l << endl;
   
  // Returns pitch of this chunk of data
  fprintf(stderr, "Note: ");
  cerr << freque[l] << endl; 

  // Reads in next chunk of data 
  fread(sI, 2, size, in); 
  for(j = 0; j < size; j++)
   d1[j] = sI[j]; 
 }
 
}
