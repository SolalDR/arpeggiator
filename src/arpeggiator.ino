#include <stdlib.h>
#include <arduino.h>
#include "MemoryFree.h"
#include "NoteStack.h"
#include "Pass.h"
#include "Melody.h"
#include "Rythmic.h"
#include "./constants.h"
#include "./controllers/lcd.h"
#include "./controllers/keyboard.h"

NoteStack noteStack;
Rythmic rythmicStack;
Melody melody;
Configuration configuration;

// Clocking
float time;
float lastTick;
float timeBetweenNote;

float getTimeBetweenNote() {
  return 60.0 / (float) configuration.bpm * configuration.rate * 1000.0;
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);   // Port série
  } else {
    Serial.begin(31250);  // MIDI baud rate:
  }

  setupKeyboard();
  setupLCD();

  uint32_t time = millis();
  lastTick = -1000;
  timeBetweenNote = getTimeBetweenNote();

  configuration.rate = rate1_4;

  melody.fundamental = 0;
  melody.variation = 2;
  melody.octave = 1;
  melody.octaveLength = 3;
  melody.direction = DIR_ASC;
  // Example
  melody.addDegree(1);
  melody.addDegree(2);
  melody.addDegree(3);
  melody.addDegree(4);
  melody.addDegree(5);
}

void loop() {
  float deltaTime = abs(time - lastTick);
  time = millis();
  timeBetweenNote = getTimeBetweenNote();
  if (timeBetweenNote > configuration.tick && deltaTime > timeBetweenNote) {
    rythmicStack.advance();
    RythmicTick * tick = rythmicStack.computeTick();

    if (tick != NULL) {
      int note = melody.advance();
      if (note >= 0) {
        noteStack.addNote(note, tick->velocity, time + timeBetweenNote * (float) tick->duration);
      }
    }

    lastTick = time;
  }

  noteStack.removeOldNotes();

  #if DEBUG && DEBUG_MEMORY
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
  #endif
  
    // Serial.println(configuration.bpm);
  // Serial.println(configuration.rate);
  loopLCD(time, &configuration, &melody);
  loopKeyboard();
  delay(configuration.tick);
}
