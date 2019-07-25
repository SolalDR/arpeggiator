#include "NoteStack.h"
#include "config.h"
using namespace std;

/*
 * Send a midi note
 * cmd: int => the channel 
 * pitch: int => [21 - 127] 
 * velocity: int
 */ 
void noteOn(int cmd, int pitch, int velocity) {
  if (DEBUG == false) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity); 
  }

  #if DEBUG && DEBUG_NOTESTACK
    Serial.print("Note :");
    Serial.print(pitch);
    Serial.print("; Velocity :");
    Serial.println(velocity);
  #endif
}

/**
 * Rajoute une nouvelle note dans la pile
 * @param {int} pitch
 * @param {int} velocity 
 * @param {float} endTime
 */
void NoteStack::addNote(int pitch, int velocity, float endTime) {
  noteOn(0x90, pitch, velocity);

  NodeNote* node = new NodeNote();
  node->pitch = pitch;
  node->endAt = endTime;
  node->next = this->head;
  this->head = node;
  this->length++;
}

/**
 * Supprime les notes passé en date
 * Exécuter à chaque TICK
 */
void NoteStack::removeOldNotes() {
  NodeNote * current = this->head;
  NodeNote * next;

  int currentTime = millis();
  while (current != NULL) {
    next = current->next;

    if(next != NULL && next->endAt < currentTime) {      
      noteOn(0x90, next->pitch, 0x00);
      NodeNote * new_next = next->next;
      this->length--;
      free(next);
      current->next = new_next;
    }
    
    current = next;
  }
}
