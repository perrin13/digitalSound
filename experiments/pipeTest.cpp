//
//test piping
//
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
 int pdes[2];
 char *buf;
 char g[] ="";
 buf = (char *) malloc(8*sizeof(char));
 sprintf(buf, "Noob");

 pipe(pdes);
 if(fork()==0){
  read(pdes[0], buf, 8);
  fprintf(stderr, "Noob: %s\n", buf);
 }else{
  buf[2] = 'n';
  write(pdes[1], buf, 8);
 }
 
 return 0;
}
