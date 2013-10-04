

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>

#define RATE 8000
#define SIZE 8
#define LENGTH 5

typedef void (*fptr)(int signum);

FILE *fd;

void endLoop(int signum); 

int main(void){
  int i;
  int j;
  unsigned char buffer[LENGTH*RATE + 1];
  double samp[RATE];
  signal(SIGINT, (fptr)endLoop);

  for(i = 0; i < RATE; i++){
   samp[i] = sin(440*(2.0*M_PI/RATE)*i);
  }
  
  for(i = 0; i < RATE; i++){
   for(j = 0; j < LENGTH; j++){
   buffer[j*RATE+i] = ((int)(((samp[i] + 2)/3)*255));
   }
  }
  
  fd = fopen("/dev/dsp", "w");
  fwrite(buffer, sizeof(buffer), 1, fd);
  fprintf(stderr, "noob");
  fwrite(buffer, sizeof(buffer), 1, fd);
  fclose(fd); 
  
  return 0;
}

void endLoop(int signum){
 fclose(fd);
 exit(0);
}
