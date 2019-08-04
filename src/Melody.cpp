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

/**
 * A chaque tick, fais avancer la mélodie
 * @return {int} Retourne le pitch de la note midi
 */
int Melody::advance() {
  if (this->passHead) {
    // Si la pass est terminé
    if (this->passNoteIndex >= this->passHead->notesLenght - 1) {
      this->advancePass();
    }
    this->passNoteIndex++;
    
    PassNote * note = this->passHead->getPassNoteAt(this->passNoteIndex); 
    if(note != NULL) {
      return this->getMidiNote(note);
    }
  }
  return -1;
}

/**
 * Génère une nouvelle pass automatiquement en suivant la précédente
 */
void Melody::advancePass() {
  Pass * nextPass = createNewPass(this->passHead, this);

  // Libère la mémoire de la pass courante
  if(this->passHead != NULL) {
    this->passHead->clear();
  }
  delete this->passHead;

  // Réassigne
  this->passHead = nextPass;
  this->passNoteIndex = -1;
}

/**
 * Calcul le pitch MIDI depuis le degrée et l'octave de la note courante et les paramètres de la mélodie
 * @param  {PassNote*} note
 * @return {int} Le pitch midi
 */
int Melody::getMidiNote(PassNote * note) {
  return BASE_NOTE_A0 + this->fundamental + this->octave*12 + note->octave*12 + this->mode[note->degree];
}

/**
 * Récupère la touche enregistrer au rang X
 * @param  {int} rank
 */
InputNode * Melody::getInputAt(int rank) {
  InputNode * current = this->inputHead;

  int index = 0;
  while (current != NULL) {
    if (index == rank + 1) {
      return current;
    }
    current = current->next;
    index++;
  }

  return NULL;
}

/**
 * Ajoute un degree à la mélodie
 */
void Melody::addDegree(int degree) {
  InputNode * current = this->inputHead;

  // Crée le nouvel input
  InputNode * newNode = new InputNode();
  newNode->degree = degree;
  newNode->next = NULL;

  // Si l'input existe déjà, return
  while (current->next != NULL) {
    current = current->next;
    if (current->degree == degree) {
      return;
    }
  }

  current->next = newNode;
  this->inputLength++;

  // Met à jour la passe courante
  this->updatePasses();
}

/**
 * Supprime un degrée à la mélodie
 */
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

/**
 * Modifie la passe courante
 * Utiliser lors des changement d'état de la mélodie
 */
void Melody::updatePasses() {
  Pass * newPass = createNewPass(this->passHead, this);

  if (this->passHead != NULL) {
    this->passHead->clear();  
  }
  
  delete this->passHead;
  this->passHead = newPass;
}

/**
 * Methode de debug
 */
void Melody::debug() {
  #if DEBUG && DEBUG_MELODY
    String debug = String("┌───────── MELODY ─────────\n");
    debug +=  String("├ Inputs (count : ") + this->inputLength + String(")\n");
    debug +=  String("├ variation: ") + this->variation + String("\n");
    debug +=  String("├ octaveLength: ") + this->octaveLength + String("\n");
    debug +=  String("├ direction: ") + this->direction + String("\n");
    debug +=  String("├ fundamental: ") + this->fundamental + String("\n");
    debug +=  String("├ octave: ") + this->octave + String("\n");
    debug +=  String("└───────── MELODY ─────────\n");

    Serial.println(debug);
  #endif
}
