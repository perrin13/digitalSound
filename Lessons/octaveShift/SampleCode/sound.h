/**
 * sound.h
 * John Brock
 * Date Created: 10 Jul 2007
 * Date Updated: 14 Jul 2007
 * Purpose: This file holds the specifications for a class that handles
 *            sound files
 * Currently Supported File Types:
 *                    -Uncompressed PCM wave file
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

  //
  // void setInput(FILE *)
  // Purpose: To change the sound file represendted by the sound class
  // Parameters: FILE * - The file that in will be changed to
  // Note: To allow for more flexibility, the in file will not be freed, and 
  //          soundParse will not be run for you
  // 
  void setInput(FILE *);
  
  // 
  // void soundParse()
  // Purpose: To parse class attributes from the file header
  // Note: Files must currently be parsed before they can be played
  //
  void soundParse();
  
  //
  // void soundPlay()
  // Purpose: To play a sound file
  // 
  void soundPlay();                
  
  // 
  // void soundRecord(sound, char *) 
  // Purpose: To record a sound file based on an input sound file
  // Parameters: sound - Class attributes are set based on this class instance
  //             char * - A string that defines the file name of the recorded sound
  // 
  void soundRecord(sound, char *);

  // Private variable access
  FILE *getFile();    // Returns a pointer to the sound file
  int getSize();      // Returns the size of the current chunk (normally data)
  int getRate();      // Returns the sampling rate
  
 private:
 
  //  Type specific parsers
  void wavParse();    // Parses a wave file header when called by soundParse 

  // Type specific players
  void wavPlay();     // Plays a wave file when called by soundPlay
 
  // Type specific recorders
  void wavRec();      // Records a wave file when called by soundRecord

  // Class specific variables
  FILE *in;
  char buf[5];        // used as a buffer for input
  string fType;       // contains the file type
  short int type;     // contains the subtype of the file
  string chunk;       // contains name of current chunk
  short int channels; // contains number of channels (mono, stereo, ect)
  int rate;           // Contains the sample rate
  short int block;    // Number of bytes per sample
  int chunkS;         // Chunk data sixe
};
