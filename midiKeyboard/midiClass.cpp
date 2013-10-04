/**
 * midiClass.cpp
 * By: John Brock
 * Created: 19 June 2008
 * Updated: 19 June 2008
 * Purpose: To implement features of midClass.h
**/
#include "midiClass.h"

// The following are implementations of midiEvent functions
unsigned char midiEvent::getEventType(){
 return (statusByte/16);
}

unsigned char midiEvent::getMidiChannel(){
 return((statusByte*16)/16);
}

void midiEvent::setEventType(unsigned char c){
 statusByte = (statusByte*16)/16 + (c*16); 
} 

void midiEvent::setMidiChannel(unsigned char c){
 statusByte = statusByte/16*16 + c;
}

unsigned char midiEvent::getEventData(){
 return dataByte;
}

void midiEvent::setEventData(unsigned char c){
 dataByte = c;
}

unsigned char midiEvent::getEventData2(){
 return dataByte;
}

void midiEvent::setEventData2(unsigned char c){
 dataByte2 = c;
}

void midiEvent::setEvent(unsigned char c){
 statusByte = c;
}

unsigned char midiEvent::getEvent(){
 return statusByte;
}

