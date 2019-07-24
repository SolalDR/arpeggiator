#include "Melody.h"
#include "config.h"
using namespace std;

void PassNote::clear() {
  if(this->next != NULL) {
    this->next->clear();
  }
  delete this->next;
}

void Pass::clear() {
  if(this->next != NULL) {
    this->next->clear();
  }
  delete this->next;
  if(this->noteHead != NULL) {
    this->noteHead->clear();
  }
  this->direction = 0;
  this->rank = -1;
  delete this->noteHead;
}


void updateFromASC(Pass* startPass, Melody* melody) {
  Pass* nextPass = (startPass->next == NULL)
    ? new Pass()
    : startPass->next;

  nextPass->clear();



  // // each pass
  // for(int i=0; i<melody->octave; i++) {
  //   currentPass->rank = i;
  //   currentPass->direction = DIR_ASC;

  //   PassNote * note = currentPass->noteHead;

  //   if(note == NULL) {
  //     note = new PassNote();
  //   }

  //   // each note
  //   for(int j=0; j<melody->inputLength; j++) {
  //     note->degree = melody->getInputAt(j)->degree;
  //     note = note->next;
  //   }

  //   if(currentPass->next) {
  //     currentPass = 
  //   }
  // }
}

InputNode* Melody::getInputAt(int rank) {
  InputNode * current = this->inputHead;
  int index = 0;
  while(current != NULL) {
    if(index == rank) {
      return current;
    }
    current = current->next;
  }
}

void Melody::addDegree(int degree) {
  InputNode * current = this->inputHead;
  
  while(current->next != NULL) {
    current = current->next;
    if(current->degree == degree) {
      return;
    }
  }

  current->next = new InputNode();
  current->next->degree = degree;
  current->next->next = NULL;

  this->updatePasses();
  this->inputLength++;
}

void Melody::removeDegree(int degree) {
  InputNode * current = this->inputHead;
  InputNode * next = 0;

  while(current != NULL) {
    next = current->next;

    if(next->degree == degree) {
      this->inputLength--;
      current->next = next->next;
      free(next);
      return;
    }

    current = next;
  }

  this->updatePasses();
}

void Melody::updatePasses() {
  Pass * startPass = this->passHead;

  // // Pas de pass courante
  // if (startPass == NULL) {
  //   this->passHead = new Pass();
  //   startPass = this->passHead;
  
  // // Pas de passe à suivre après la courante
  // } else if(startPass->next == NULL) {
  //   startPass->next = new Pass();
  //   startPass = startPass->next;
  
  // // Next pass is defined
  // } else {
  //   startPass = startPass->next;
  // }

  switch(this->direction) {
    case DIR_ASC: updateFromASC(startPass, this); break;
  }
}



void Melody::debug() {
  Serial.println("------- MELODY -------");
  Serial.print("--- Inputs (count : ");
  Serial.print(this->inputLength);
  Serial.println(")");

  InputNode * currentInput = this->inputHead;
   while(currentInput->next != NULL) {
    
    Serial.println(currentInput->next->degree);
    currentInput = currentInput->next;
  }

  Serial.println("------- /MELODY/ -------");
}
