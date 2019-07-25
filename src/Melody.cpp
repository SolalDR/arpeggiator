#include "Melody.h"
#include "MemoryFree.h"
#include "config.h"
using namespace std;
#include "utils/asc.h"

/**
 * Permet de créer une nouvelle passes à partir de la passe précédente et de la mélodie.
 * @returns {Pass} newPass
 */
Pass * createNewPass(Pass * previousPass, Melody * melody) {
    Pass * newPass = new Pass();
    newPass->direction = melody->direction;
    newPass->variation = melody->variation;

    switch (newPass->direction) {
        case DIR_ASC: hydratePassASC(newPass, previousPass, melody); break;
    }

    return newPass;
}

// Pass * Melody::computeNextPass(Pass * current) {
//   Pass * next = new Pass();
//   next->direction = current->direction;
//   next->variation = current->notesLenght;
// }


int Melody::advance() {
  if (this->passHead) {

    // Last item
    if (this->passNoteIndex >= this->passHead->notesLenght - 1) {
      this->advancePass();
    }
    this->passNoteIndex++;
    
    return this->getMidiNote(
      this->passHead->getPassNoteAt(this->passNoteIndex)
    );
  }
  return -1;
}

void Melody::advancePass() {
  Pass * nextPass = createNewPass(this->passHead, this);

  if(this->passHead != NULL) {
    this->passHead->clear();
  }

  delete this->passHead;
  this->passHead = nextPass;
  this->passNoteIndex = -1;
}

int Melody::getMidiNote(PassNote * note) {
  return BASE_NOTE_A0 + this->fundamental + this->octave*12 + note->octave*12 + this->mode[note->degree];
}

InputNode* Melody::getInputAt(int rank) {
  InputNode * current = this->inputHead;

  int index = 0;
  while(current != NULL) {
    if(index == rank + 1) {
      return current;
    }
    current = current->next;
    index++;
  }
}

void Melody::addDegree(int degree) {
  InputNode * current = this->inputHead;

  InputNode * newNode = new InputNode();
  newNode->degree = degree;
  newNode->next = NULL;

  while(current->next != NULL) {
    current = current->next;
    if(current->degree == degree) {
      return;
    }
  }

  current->next = newNode;

  this->inputLength++;
  this->updatePasses();
}

void Melody::removeDegree(int degree) {
  InputNode * current = this->inputHead;
  InputNode * next = 0;

  while(current != NULL) {
    next = current->next;

    if(next->degree == degree) {
      this->inputLength--;
      current->next = next->next;
      delete next;
      this->updatePasses();
      return;
    }
    current = next;
  }
}

void Melody::updatePasses() {
  Pass * newPass = createNewPass(this->passHead, this);

  if(this->passHead != NULL) {
    this->passHead->clear();  
  }
  
  delete this->passHead;
  this->passHead = newPass;
}


void Melody::debug() {
  #if DEBUG && DEBUG_MELODY
    Serial.println("------- MELODY -------");
    Serial.print("| --- Inputs (count : ");
    Serial.print(this->inputLength);
    Serial.println(")");
    InputNode * currentInput = this->inputHead;
    Serial.print("| ");
    while(currentInput->next != NULL) {
      Serial.print(currentInput->next->degree);
      Serial.print(", ");
      currentInput = currentInput->next;
    }
    Serial.println("");
    Serial.print("| variation: ");
    Serial.println(this->variation);
    Serial.print("| octaveLength: ");
    Serial.println(this->octaveLength);
    Serial.print("| direction: ");
    Serial.println(this->direction);
    Serial.print("| fundamental: ");
    Serial.println(this->fundamental);
    Serial.print("| octave: ");
    Serial.println(this->octave);
    Serial.println("|------- /MELODY/ -------");
    Serial.println("");
  #endif
}
