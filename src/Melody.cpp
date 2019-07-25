#include "Melody.h"
#include "config.h"
using namespace std;


/**
 * Recalcule la passe courante
 */
void updatePassASC(Pass* startPass, Melody* melody) {
  Pass* nextPass = 0;

  // Si il n'y a aucune passe de joué en ce moment, le nextPass devient le premier
  if(startPass == NULL) {
    startPass = new Pass();
    nextPass = startPass;
    melody->passHead = startPass;

  // Sinon on doit soit créer une passe suivante ou récupérer celle déjà existante pour la réécrire
  } else {
    nextPass = (startPass->next == NULL)
      ? new Pass()
      : startPass->next;
  }

  // Supprime tous les enfants de la passe par récurrence
  nextPass->clear();

  nextPass->direction = DIR_ASC;
  nextPass->variation = melody->variation;

  // Dans les variation 2 et 3 quand il y a plus de 4 notes, chaque octave est composé de 2 passes
  int rankMax = melody->octaveLength;
  if ((melody->variation == 2 || melody->variation == 3)
      && melody->inputLength > 4) {
    rankMax *= 2;
  }

  // Si il n'y avait aucune passe de jouée
  // Ou que la direction n'était pas ascendante
  // Ou que la variation était différente
  // On recommence du début
  if (
    startPass == NULL 
    || startPass->direction != DIR_ASC 
    || startPass->variation != nextPass->variation) {
    nextPass->rank = 0;
  } else 

  // Sinon on continue on progresse dans la séquence de passes 
  if(startPass->direction == DIR_ASC) {
    nextPass->rank = startPass->rank % rankMax;

  // Recommence au départ dans tous les autres cas 
  } else {
    nextPass->rank = 0;
  }

  // Rajoute les notes dans la passes en fonction de la variation
  
  InputNode* input = NULL;
  switch(nextPass->variation) {
    case 1:    
    for (int i=0; i < melody->inputLength; i++) {
      input = melody->getInputAt(i);
      int degree = input->degree;
      nextPass->addNote( degree, nextPass->rank + 1 );
    }
    break;

    case 2:
    // TODO
    break;

    case 3:
    // TODO
    break;
  }
}

Pass * Melody::computeNextPass(Pass * current) {
  Pass * next = new Pass();
  next->direction = current->direction;
  next->variation = current->notesLenght;
}


int Melody::advance() {
  if (this->passHead) {
    Serial.println("-----");
    
    // Last item
    Serial.println(this->passHead->notesLenght);
    Serial.println(this->passNoteIndex);
    if (this->passNoteIndex >= this->passHead->notesLenght - 1) {
      this->advancePass();
    } else {
      this->passNoteIndex++;
    }
    
    Serial.println(this->passNoteIndex);
    return this->getMidiNote(
      this->passHead->getPassNoteAt(this->passNoteIndex)
    );
  }
  return -1;
}

void Melody::advancePass() {
  // Pas de passe courante
  if(this->passHead == NULL) {
    return;
  }

  // Passe à supprimer
  Pass * oldPass = this->passHead;

  // Si il y a une prochaine passe
  if (this->passHead->next != NULL) {
    this->passHead = this->passHead->next;
  } else {
    Pass * newPass = this->computeNextPass(oldPass);
    delete oldPass;
    this->passHead = newPass;
  }

  this->passNoteIndex = -1;
  delete oldPass;
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
      return;
    }

    current = next;
  }

  this->updatePasses();
}

void Melody::updatePasses() {
  Pass * startPass = this->passHead;

  switch(this->direction) {
    case DIR_ASC: updatePassASC(startPass, this); break;
  }
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
