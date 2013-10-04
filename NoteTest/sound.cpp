/**
 * sound.cpp
 * John Brock
 * Date Created: 10 Jul 2008
 * Date Updated: 14 Jul 2008
 * Purpose: To parse a sound file into a usable form.
**/
#include "sound.h"

// Default constructor
sound::sound(){}

// Constructor with input
sound::sound(FILE *input){
 in = input;
}

// Pre-record constructor
sound::sound(sound orig, char *pC){
 FILE *temp = orig.getFile();
 fseek(temp, 0, SEEK_SET);
 unsigned char *buff;

 buff = (unsigned char *) malloc(45);
 fread(buff, 1, 44, temp);
 
 if((in = fopen(pC, "w")) == NULL){
  fprintf(stderr, "ERROR: failed to open record file\n");
  exit(1);
 } 

 fwrite(buff, 1, 44, in);
 fclose(in);
 in = fopen(pC, "r");
 soundParse();
 free(buff);
 fclose(in);
 in = fopen(pC, "a");
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
 fseek(in, 0, SEEK_SET); 
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
  cout << fType << endl;
 }
}

// Type specific players

  // Wave player
  void sound::wavPlay(){
   signed short int *samp;
   int fd;  
   fseek(in, 44, SEEK_SET);
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

// Default Record
void sound::soundRecord(char *pC){
 unsigned char *buf;

 buf = (unsigned char *) malloc(45);
 buf[0] = 0x52;
 buf[1] = 0x49;
 buf[2] = 0x46;
 buf[3] = 0x46;
 buf[4] = 0x9E;
 buf[5] = 0xE3;
 buf[6] = 0x2F;
 buf[7] = 0x0;
 buf[8] = 0x57;
 buf[9] = 0x41;
 buf[10] = 0x56;
 buf[11] = 0x45;
 buf[12] = 0x66;
 buf[13] = 0x6D;
 buf[14] = 0x74;
 buf[15] = 0x20;
 buf[16] = 0x10;
 buf[17] = 0x0;
 buf[18] = 0x0;
 buf[19] = 0x0;
 buf[20] = 0x1;
 buf[21] = 0x0;
 buf[22] = 0x1;
 buf[23] = 0x0;
 buf[24] = 0x44;
 buf[25] = 0xAC;
 buf[26] = 0x0;
 buf[27] = 0x0;
 buf[28] = 0x88;
 buf[29] = 0x58;
 buf[30] = 0x1;
 buf[31] = 0x0;
 buf[32] = 0x2;
 buf[33] = 0x0;
 buf[34] = 0x10;
 buf[35] = 0x0;
 buf[36] = 0x64;
 buf[37] = 0x61;
 buf[38] = 0x74;
 buf[39] = 0x61;
 buf[40] = 0x16;
 buf[41] = 0xE1;
 buf[42] = 0x2F;
 buf[43] = 0x0;
 
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
int sound::getSize(){
 return chunkS;
}
int sound::getRate(){
 return rate;
}
