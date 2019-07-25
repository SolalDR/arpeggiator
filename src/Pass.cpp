#include "Pass.h"
#include "config.h"
using namespace std;

/**
 * Supprime toutes les notes suivantes récursivement
 */
void PassNote::clear() {
  if(this->next != NULL) {
    this->next->clear();
  }
  delete this->next;
  this->next = NULL;
}

/**
 * Supprime toute les pass suivantes récurivement 
 */
void Pass::clear() {
  if(this->noteHead != NULL) {
    this->noteHead->clear();
  }
  this->direction = 0;
  this->rank = -1;
  delete this->noteHead;
  this->noteHead = NULL;
}

/**
 * Retourne 
 */
PassNote* Pass::getPassNoteAt(int rank) {
  PassNote * current = this->noteHead;

  int index = 0;
  while(current != NULL) {
    if(index == rank) {
      return current;
    }
    current = current->next;
    index++;
  }
}

void Pass::addNote(int degree, int octave) {
  PassNote * current = this->noteHead;
  
  PassNote * note = new PassNote();
  note->degree = degree;
  note->octave = octave;
  note->next = NULL;
  this->notesLenght++;

  if (current == NULL) {
    this->noteHead = note;
    return;
  }

  while(current->next != NULL) {
    current = current->next;
    if(current->degree == degree) {
      return;
    }
  }

  current->next = note;
}


void PassNote::debug(bool recursive) {
  #if DEBUG && DEBUG_MELODY
    Serial.println("------ Note ------");
    Serial.print("| degree: ");
    Serial.println(this->degree);
    Serial.print("| octave: ");
    Serial.println(this->octave);
    Serial.println("------ /Pass/ ------");
    Serial.println("");
  #endif
  if (recursive && this->next != NULL) {
    this->next->debug(true);
  }
}

void Pass::debug(bool debugNotes) {
  #if DEBUG && DEBUG_MELODY
    Serial.println("------ Pass ------");
    Serial.print("| direction: ");
    Serial.println(this->direction);
    Serial.print("| rank: ");
    Serial.println(this->rank);
    Serial.print("| notes count: ");
    Serial.println(this->notesLenght);
    Serial.print("| variation: ");
    Serial.println(this->variation);
    Serial.print("| noteHead: ");
    Serial.println((int) this->noteHead);
    Serial.print("| nextPass: ");
    Serial.println((int) this->next);
    Serial.println("------ /Pass/ ------");
    Serial.println("");
  #endif
  if (debugNotes && this->noteHead != NULL) {
    this->noteHead->debug(true);
  }
}