/**
 * midiInput.cpp
 * By: John Brock
 * Date Created: 1 July 2008
 * Date Updated: 1 July 2008
 * Purpose: To interpret input from a midi control surface
 *             -Print midi commands to stdout
 *             -Play midi notes entered
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
typedef void (*fptr)(int signum);
FILE *fd;

int main(){
 unsigned char c[4];
 
 if((fd = fopen("/dev/snd/midiC1D0", "r")) == NULL){
  exit(1);
 }
 
 while(true){
  fread(c, 1, 1, fd);
  if(((int)c[0]) & 0x80){
   fread(&c[1], 1, 1, fd);
   fprintf(stdout, "Command: %0X\n Data1: %0X\n", c[0], c[1]);
   if((((int)c[0])&0xC0) != 0xC0 && (((int)c[0])&0xD0) != 0XD0){
    fread(&c[2], 1, 1, fd);
    fprintf(stdout, " Data 2: %0X\n", c[2]);
   } 
   if(((int)c[0]&0x90) == 0x90)
    
  }
 }
 
 fclose(fd);
 return 0;
}

void endLoop(int signum){
 fclose(fd);
 exit(0);
}
