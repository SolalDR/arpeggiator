#ifndef Morse_h
#define Morse_h

#include "arduino.h"

class NodeNote {
  public: 
    int pitch;
    int endAt;
    NodeNote* next;
};


class NoteStack {  
  private:
    NodeNote* head;
    int length;
    

  public: 
    NoteStack();
    void addNote(int pitch, int velocity, float endTime);
    void removeOldNotes(void);
};

#endif

