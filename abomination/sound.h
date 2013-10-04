/**
 * sound.h
 * John Brock
 * Date Created: 10 Jul 2007
 * Date Updated: 14 Jul 2007
 * Purpose: This file holds the specifications for a class that handles
 *            sound files
 * Currently Supported File Types:
 *                    -
**/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <math.h>
using namespace std;

class sound{
 public:
  // Constructors
  sound();
  sound(FILE *);
  sound(sound, char *);
 
  // Destructors
  ~sound();
 
  // functions
  void setInput(FILE *);
  void soundParse();
  void soundPlay();
  void soundRecord(sound, char *);

  // Private variable access
  FILE *getFile();
  int getSize();
  int getRate();
  
 private:
 
  //  Type specific parsers
  void wavParse();

  // Type specific players
  void wavPlay();
 
  // Type specific recorders
  void wavRec();

  // Class specific variables
  FILE *in;
  char buf[5];   // used as a buffer for input
  string fType;  // contains the file type
  short int type;     // contains the subtype of the file
  string chunk;  // contains name of current chunk
  short int channels;  // contains number of channels (mono, stereo, ect)
  int rate;      // Contains the sample rate
  short int block; // Number of bytes per sample
  int chunkS;    // Chunk data sixe
};
