#ifndef NodeNote_h
#define NodeNote_h

#include "arduino.h"

/**
 * Une note jouée dans la pile
 * Toutes les notes sont liés entre elles par un pointeur
 * @property {int} pitch La note MIDI 
 * @property {int} endAt Moment dans l'horloge ou la note sera arrêté
 * @property {NodeNote*} next Un pointeur vers la note suivante
 */
class NodeNote {
  public: 
    int pitch;
    int endAt;
    NodeNote* next;
};

/** 
 * Une pile de note
 */
class NoteStack {  
  private:
    NodeNote* head = NULL;
    int length = 0;

  public: 
    void addNote(int pitch, int velocity, float endTime);
    void removeOldNotes(void);
};

#endif