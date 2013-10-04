/**
 * vocode.cpp
 * By: John Brock
 * Date Created: 14 July 2008
 * Date Updated: 16 July 2008
 * Purpose: To create a vocoded file based on an inputed and a recorded file
**/
#include "sound.h"
short FFT(short int dir,long m,double *x,double *y);
void vocode(FILE *nub, sound &noob, char *pC);
#define size 512

int main(int argv, char *argc[]){
 char *pC;
 FILE *input;
 FILE *input2;
 sound nub;
 
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
 fprintf(stderr, "Made it this far\n");
 // noob.soundPlay();
 if(argv > 2){
  pC = argc[2];
  if((input2 = fopen(pC, "r")) == NULL){
   fprintf(stderr, "ERROR: Failed to open file %s\n", pC);
   exit(1);
  }
  sound nub(input2); 
  nub.soundParse();
  // nub.soundPlay();
 }
 if(argv > 3){
  pC = argc[3];
  vocode(input2, noob, pC);
 }
 fclose(input);
 return 0;
}

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

void vocode(FILE *temp2, sound &noob, char *pC){
 short int *sI;
 int i, j;
 double *d;
 double *d1;
 double *d2;
 double *d1t;
 double *d2t;
 double *complex1;
 double *complex2;
 double big;
 sound voco;
 FILE *in;
 FILE *temp = noob.getFile();
 fseek(temp, 0, SEEK_SET);
 fprintf(stderr, "Passed 1\n");
 unsigned char *buf;

 buf = (unsigned char *) malloc(45);
 fread(buf, 1, 44, temp);
 
 if((in = fopen(pC, "w")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 

 fseek(in, 0, SEEK_SET);
 fprintf(stderr, "Passed 3\n");
 fwrite(buf, 1, 44, in);
 fclose(in);
 if((in = fopen(pC, "r")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 
 voco.setInput(in);
 voco.soundParse();
 free(buf);
 fclose(in);
 if((in = fopen(pC, "a")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 
 
 // FILE *temp2 = nub.getFile();
 fseek(temp2,  0, SEEK_SET);
 fprintf(stderr, "Passed 4\n");
 d = (double *) calloc(size,sizeof(double));
 d1 = (double *) malloc(sizeof(double)*size);
 d2 = (double *) malloc(sizeof(double)*size);
 complex1 = (double *) malloc(sizeof(double)*size);
 complex2 = (double *) malloc(sizeof(double)*size);
 sI = (short int *) malloc(sizeof(short int)*size);
 //d1t = (double *) malloc(sizeof(double)*size);
 //d2t = (double *) malloc(sizeof(double)*size);

 fread(sI, 2, size, temp2); 
 for(j = 0; j < size; j++)
  d1[j] = sI[j];
  
 fread(sI, 2, size, temp);
 for(j = 0; j < size; j++)
  d2[j] = sI[j];
 
 for(i = 0; i < noob.getSize(); i += size/4){
  for(j = 0; j < size; j++){
   complex1[j] = 0;
   complex2[j] = 0;
   //d2t[j] = d2[j];
   //d1t[j] = d1[j];
  } 

  FFT(1, ((int)log2(size)), d1, complex1);
  FFT(1, ((int)log2(size)), d2, complex2);
  for(j = 0; j < (size/2); j++){
   d2[j] = d1[j]*d2[j] - complex1[j]*complex2[j];
   complex2[j] = d1[j]*complex2[j] + d2[j]*complex1[j];
  }
  FFT(-1, ((int)log2(size)), d2, complex2);
  
  big = max_Double(d2, size);
  for(j = 0; j < size; j++){
   // d[j] = 32767*d2[j]/big;
   /*if(i == 0){
    if(j > 3*size/4)
     d[j] = d[j] + d[j];
    if(j > size/2)
     d[j] = d[j] + d[j];
    if(j > size/4)
     d[j] = d[j] + d[j];
   }*/
   sI[j] = ((short int)(32767*d2[j]/big));
  }
  fwrite(sI, 2, (size), in);
  
  /*for(j = 0; j < (3*size/4); j++){
   d[j] = d[size/4 + j];
   d1[j] = d1t[size/4 + j];
   d2[j] = d2t[size/4 + j];
  }
  for(j = 3*size/4; j < size; j++){
   d[j] = 0;
   d1[j] = 0;
   d2[j] = 0;
  }*/
  fread(sI, 2, size, temp2); 
  for(j = 0; j < size; j++)
   d1[j] = sI[j];
  
  fread(sI, 2, size, temp);
  for(j = 0; j < size; j++)
   d2[j] = sI[j]; 
 }
 fclose(in);
 if((in = fopen(pC, "r")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 

 voco.setInput(in);
 voco.soundParse();
 // voco.soundPlay();
}

/*
   This computes an in-place complex-to-complex FFT 
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform 
*/
short FFT(short int dir,long m,double *x,double *y)
{
   long n,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   }
   
   return(1);
}

