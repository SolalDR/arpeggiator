#include "Melody.h"
#include "config.h"
using namespace std;

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