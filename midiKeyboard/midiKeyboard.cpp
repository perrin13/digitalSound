/**
 * midiKeyboard.cpp
 * John Brock
 * Start Date: 19 June 2008
 * Last Update: 19 June 2008
 * Purpose: To create a virtual midi keyboard using the keyboard
**/
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
#include "midiClass.h"

// Function Prototypes
string *m_textParse(string note);
midiEvent *midiNote(string midi, unsigned char channel);

int main(void){
  string *seq;
  string note;
  int i;
  unsigned char channel = 0;
 
  cout << "Input a midi seq in format 3cfgb, where 3 is the octave followed by notes : " << endl;
  cin >> note;
  seq = m_textParse(note);

  for(i = 0; seq[i] != "NULL"; i++){
   if(seq[i][0] == 'z'){
    channel = seq[i][1] - '0';
    continue;
   } 
   cout << " Note: " <<  seq[i] << endl;
   midiEvent *nub = midiNote(seq[i], channel);
   fprintf(stderr, " Event: %0X \n", nub->getEvent()); 
   fprintf(stderr, " Data: %0X \n\n", nub->getEventData());
   
  }
  
  return 0;
}

//
// string *m_textParse(string midi)
// Purpose: To convert a string into an a array of notes readable
//           by midiText
// Parameters: string midi - string to be parsed
// Returns: string * - returns an array of strings
//
string *m_textParse(string midi){
 int i = 0 , j = 0, k = 0;
 string parsed = "";
 unsigned char c = midi[j];
 unsigned char q;
 
 if(c == 0){
  fprintf(stderr, "ERROR: empty midi string");
 }
 
 while(c != 0){
  if(c >= '0' && c <= '9'){
   q = c;
   c = midi[++j];
   while(c > '9'){
    parsed.push_back(c);
    parsed.push_back(q);
    i++;
    c = midi[++j];
    if(c == '#' || c == '@'){
     parsed.push_back(c);
     c = midi[++j];
    }
   }
  }
 }
 
 string *seq = new string [i + 1];
 for(j = 0; j < i; j++){
  seq[j] = "";
  c = parsed[j*2 + k];
  seq[j].push_back(c);
  c = parsed[j*2+1 + k];
  seq[j].push_back(c);
  if((c = parsed[j*2+2+k]) == '#' || c == '@'){
    seq[j].push_back(c);
    k++;
  }
 }
 seq[i] = "NULL";
 return seq;
}

//
// midiEvent midiNote(string midi, unsigned char channel)
// Purpose: Converts a string into a midi note
// Parameters: string midi - string to be converted
//             unsigned char channel - holds midi channel
// Return:  midiEvent  -  return midiEvent
//
midiEvent *midiNote(string midi, unsigned char channel){
 // parse midi
 char m = midi[0] - 'a';
 
 switch(m){
  case 0: m = 9; break;
  case 1: m = 11; break;
  case 2: m = 0; break;
  case 3: m = 2; break;
  case 4: break;
  case 5: break;
  case 6: m = 7; break;
  default: fprintf(stderr, "ERROR: improper note format"); exit(1); break;
 }
 
 m = m + 12*(midi[1] - 3 - '0') + 0x3C;
 if(midi[2] == '#')
   m++;
 if(midi[2] == '@')
   m--;
 midiEvent *noteon = new midiEvent;
 noteon->setEventType(0x9);
 noteon->setMidiChannel(channel);
 noteon->setEventData(m);
 noteon->setEventData2(67);

 return noteon;
}

