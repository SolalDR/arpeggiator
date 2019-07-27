#include "Pass.h"
#include "config.h"
using namespace std;

/**
 * Supprime toute les pass suivantes récurivement 
 */
void Pass::clear() {
  if(this->noteHead != NULL) {
    // Supprime toute les notes récursivement
    this->noteHead->clear();
  }
  // Reset
  this->direction = 0;
  this->rank = -1;

  // Supprime la référence du pointer
  delete this->noteHead;
  this->noteHead = NULL;
}

/**
 * Rajoute une note au bout de la pass
 * @param {int} degree [0 - 7]
 * @param {int} octave [1 - 4]
 */
void Pass::addNote(int degree, int octave) {
  PassNote * current = this->noteHead;
  
  // Crée la note
  PassNote * note = new PassNote();
  note->degree = degree;
  note->octave = octave;
  note->next = NULL;
  this->notesLenght++;

  // Si pas de noteHead
  if (current == NULL) {
    this->noteHead = note;
    return;
  }

  // Sinon met au bout de la liste
  while(current->next != NULL) {
    current = current->next;
  }
  current->next = note;
}

/**
 * Récupère la note présente au rank "rank"
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
  return NULL;
}

/**
 * Debug une passe
 * @param  {boolean} debugNotes Si "true" appellera aussi les méthode de debug des notes contenu dans la passe
 */
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
    Serial.println("------ /Pass/ ------");
    Serial.println("");
  #endif
  if (debugNotes && this->noteHead != NULL) {
    this->noteHead->debug(true);
  }
}

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
 * Debug une note
 * @param  {boolean} recursive Si "true" la méthode de débug iterera dans toute les notes suivantes
 */
void PassNote::debug(bool recursive) {
  #if DEBUG && DEBUG_MELODY
    Serial.println("------ Note ------");
    if((int) this == 0) {
      Serial.println("| NULL");
    } else {
      Serial.print("| degree: ");
      Serial.println(this->degree);
      Serial.print("| octave: ");
      Serial.println(this->octave);
    }
    Serial.println("------ /Pass/ ------");
    Serial.println("");
  #endif
  if (recursive && this->next != NULL) {
    this->next->debug(true);
  }
}