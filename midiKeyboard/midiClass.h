/** 
 * midiClass.h
 * By: John Brock
 * Date Created: 19 June 2008
 * Purpose: To contain and control midi data
**/
#include  <iostream>

#ifndef MIDICLS
#define MIDICLS
class midiEvent{
 public:
  unsigned char getEventType();
  unsigned char getMidiChannel();
  void setEventType(unsigned char); 
  void setMidiChannel(unsigned char);

  unsigned char getEvent();
  void setEvent(unsigned char);
  
  unsigned char getEventData();
  void setEventData(unsigned char);
  unsigned char getEventData2();
  void setEventData2(unsigned char);
 protected:
 private:
  unsigned char statusByte;
  unsigned char dataByte;
  unsigned char dataByte2;
};
 
#endif
