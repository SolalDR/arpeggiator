#include <stdlib.h>
#include <arduino.h>
#include "config.h"
#include "MemoryFree.h"
#include "NoteStack.h"
#include "Pass.h"
#include "Melody.h"
#include "Rythmic.h"
#include "./constants.h"

NoteStack noteStack;
Rythmic rythmicStack;
Melody melody;

// Clocking
float time;
float lastTick;
float timeBetweenNote;
int bpm;
float tempo;

float getTimeBetweenNote() {
  return 60.0 / (float) bpm * tempo * 1000.0;
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);   // Port série
  } else {
    Serial.begin(31250);  // MIDI baud rate:
  }

  time = millis();
  lastTick = -1000;
  bpm = 15;
  tempo = temp1_1;
  timeBetweenNote = getTimeBetweenNote();

  melody.fundamental = 3;
  melody.octave = 3;
  melody.octaveLength = 3;

  // Example
  melody.addDegree(0);
  melody.addDegree(2);
  melody.addDegree(4);
}

void loop() {
  float deltaTime = abs(time - lastTick);
  time = millis();

  if (deltaTime > timeBetweenNote) {
    rythmicStack.advance();
    RythmicTick * tick = rythmicStack.computeTick();

    if (tick != NULL) {
      int note = melody.advance();
      if (note != NULL) {
        noteStack.addNote(note, tick->velocity, time + timeBetweenNote * (float) tick->duration);
      }
    }

    noteStack.removeOldNotes();
    lastTick = time;
  }

  #if DEBUG && DEBUG_MEMORY
    if((int) time % (int) 2000 < 5) {
      Serial.print("freeMemory()=");
      Serial.println(freeMemory());
    }
  #endif
  
  delay(TICK);
}