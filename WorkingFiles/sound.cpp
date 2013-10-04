/**
 * sound.cpp
 * John Brock
 * Date Created: 10 Jul 2008
 * Date Updated: 10 Jul 2008
 * Purpose: To parse a sound file into a usable form.
**/
#include "sound.h"

// Default constructor
sound::sound(){}

// Constructor with input
sound::sound(FILE *input){
 in = input;
}

// Default destructor
sound::~sound(){}

// Sets sound file to a user specified
void sound::setInput(FILE *input){
 in = input;
 fType = "none";
}

// Parses sound file header and format chunk
void sound::soundParse(){
 fread(buf, 1, 4, in);
 buf[4] = 0;
 fType = buf;
 if(fType == "RIFF"){
  wavParse();
  return;
 } else {
  fprintf(stderr, "ERROR: Unknown file type: ");
  cout << buf << endl;
  exit(1);
 }
}

// Type Specific parsers
 
  // Wave parser
  void sound::wavParse(){
   int i;
   fread(buf, 1, 4, in);
   fread(buf, 1, 4, in);
   buf[4] = 0;
   if(strcmp(buf, "WAVE")){
    fprintf(stderr, "ERROR:  WAVE not found\n");
    exit(1);
   }
   
   fread(buf, 1, 4, in);
   buf[4] = 0;
   chunk = buf;
   if(chunk[0] != 0x66 || chunk[1] != 0x6D || chunk[2] != 0x74){
    fprintf(stderr, "ERROR: Parser currently requires fmt chunk first\n");
    exit(1);
   }
   fread(&chunkS, 4, 1,in);

   fread(&type, 2, 1, in);
   fprintf(stderr, "Compression type: %0X\n", type);
   if(type != 1){
    fprintf(stderr, "ERROR: Unsupported wav type %d\n", type);
    exit(1);
   }
   
   fread(&channels, 2, 1, in);
   fprintf(stderr, "Channels: %0X\n",  channels);
   fread(&rate, 4, 1, in);
   fprintf(stderr, "Rate: %0X\n", rate);
   fread(buf, 1, 4, in);
   fread(&block, 2, 1, in);
   fprintf(stderr, "Block: %0X\n", block);
   fread(buf, 1, 2, in);
   
   fread(buf, 1, 4, in);
   buf[4] = 0;
   fprintf(stderr, "Next Chunk: %s", buf);
   chunk = buf;
   if(chunk != "data"){
    fprintf(stderr, "ERROR: Only data chunks are currently supported\n");
    exit(1);
   }
  
   fread(&chunkS, 4, 1, in);
   fprintf(stderr, "Data Size: %0X\n", chunkS);
  }   

// Plays sound file
void sound::soundPlay(){
 if(fType == "RIFF"){
  wavPlay();
  return;
 } else {
  fprintf(stderr, "ERROR: ok this is totally not my fault\n");
 }
}

// Type specific players

  // Wave player
  void sound::wavPlay(){
   signed short int *samp;
   int fd;  
   
   if((fd = open("/dev/dsp", O_WRONLY, 0)) == -1){
    perror("/dev/dsp");
    exit(1);
   }  
  
   int format;
   if(block == 2){
   format = AFMT_S16_LE; 
   if (ioctl(fd, SNDCTL_DSP_SETFMT, &format)==-1){
     perror("SNDCTL_DSP_SETFMT"); 
     exit(1); 
   }  }
 
   if (ioctl(fd, SNDCTL_DSP_STEREO, &channels)==-1){
    perror("SNDCTL_DSP_STEREO");
    exit(1);
   }

   if (channels != 1){
    //The device doesn't support stereo mode.
   }
   
   if (ioctl(fd, SNDCTL_DSP_SPEED, &rate)==-1){ 
    perror("SNDCTL_DSP_SPEED"); 
    exit(1); 
   }  
    
   samp = (signed short int *) malloc(chunkS+1);
   
   fread(samp, 2, (chunkS/2), in);
   fprintf(stderr, "Playing in progress...\n");
   write(fd, samp, chunkS);
   close(fd);
   fprintf(stderr, "Playing finished!\n");
  }


// Recorder
void sound::soundRecord(sound noob, char *pC){
 FILE *temp = noob.getFile();
 fseek(temp, 0, SEEK_SET);
 unsigned char *buf;

 buf = (unsigned char *) malloc(45);
 fread(buf, 1, 44, temp);
 
 if((in = fopen(pC, "w")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 

 fseek(in, 0, SEEK_SET);
 fwrite(buf, 1, 44, in);
 fclose(in);
 in = fopen(pC, "r");
 soundParse();
 free(buf);
 fclose(in);
 in = fopen(pC, "a");

 if(fType == "RIFF"){
  wavRec();
  fclose(in);
  in = fopen(pC, "r");
  return;
 } else {
  fprintf(stderr, "ERROR: ok this is totally not my fault 2.0\n");
  exit(0);
 }
}

// Type specific recorders

  // Wave recorders
  void sound::wavRec(){
   signed short int *samp;
   int fd;  
   
   if((fd = open("/dev/dsp", O_RDONLY, 0)) == -1){
    perror("/dev/dsp");
    exit(1);
   }  
  
   int format;
   if(block == 2){
   format = AFMT_S16_LE; 
   if (ioctl(fd, SNDCTL_DSP_SETFMT, &format)==-1){
     perror("SNDCTL_DSP_SETFMT"); 
     exit(1); 
   }  }
 
   if (ioctl(fd, SNDCTL_DSP_STEREO, &channels)==-1){
    perror("SNDCTL_DSP_STEREO");
    exit(1);
   }

   if (channels != 1){
    //The device doesn't support stereo mode.
   }
   
   if (ioctl(fd, SNDCTL_DSP_SPEED, &rate)==-1){ 
    perror("SNDCTL_DSP_SPEED"); 
    exit(1); 
   }  
    
   samp = (signed short int *) malloc(chunkS+1);
   fprintf(stderr, "Recording in progress...\n");
   read(fd, samp, chunkS);
   fwrite(samp, 2, chunkS/2, in);
   close(fd);
   fprintf(stderr, "Recording complete!\n");
  }

FILE * sound::getFile(){
 return in;
}
