#include <LiquidCrystal.h>
#include <arduino.h>
#include "../constants.h"

#define MODE_BPM 0
#define MODE_TONE 1
#define MODE_RATE 2
#define MODE_RYTHM 3
#define MODE_MIDI 4

#define PIN_BTN_TOP 45
#define PIN_BTN_TOP_RIGHT 51
#define PIN_BTN_BOTTOM_RIGHT 53
#define PIN_BTN_BOTTOM 49
#define PIN_BTN_BOTTOM_LEFT 43
#define PIN_BTN_TOP_LEFT 41
#define PIN_BTN_MIDDLE 47

#define RANK_BTN_TOP 0
#define RANK_BTN_TOP_RIGHT 1
#define RANK_BTN_BOTTOM_RIGHT 2
#define RANK_BTN_BOTTOM 3
#define RANK_BTN_BOTTOM_LEFT 4
#define RANK_BTN_TOP_LEFT 5
#define RANK_BTN_MIDDLE 6

LiquidCrystal lcd(42, 44, 46, 48, 50, 52);
bool buttons[] = {false, false, false, false, false, false, false};
int currentMode = MODE_TONE;
int lastLCDTick = 0;


/**
 * Toggle 
 */
void updateButtonValue(int pin, int rank) {
   int value = digitalRead(pin);
   if(!value != buttons[rank]) {
    buttons[rank] = !value;

    #if DEBUG && DEBUG_LCD_BTN_MODULE
      String base = buttons[rank] == true 
        ? String("-- Press Button => ")
        : String("-- Release Button => ");
      String debug = base + String(' rank: ') + rank + String("; pin: ") + pin;
      Serial.println(debug);
    #endif
   }
}

/**
 * Udate Tone
 */
void updateTone(Configuration * config, Melody * melody) {
  if(buttons[RANK_BTN_TOP]) {
    if (config->midiNumber < 108) {
      config->midiNumber++;
    }
  }

  if(buttons[RANK_BTN_BOTTOM]) {
    if (config->midiNumber > 21) {
      config->midiNumber--;
    }
  }

  String notes[] = NOTES;
  int rank = (config->midiNumber - 21) % 12;
  int octave = (config->midiNumber - 21) / 12;
  melody->octave = octave;
  melody->fundamental = rank;

  String letters = String(notes[rank]) + String(melody->octave); 

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Tone ");
  lcd.setCursor(16 - letters.length(), 1);
  lcd.print(letters);
}

/**
 * Update BPM
 */
void updateBpm(Configuration * config) {
  if(buttons[RANK_BTN_TOP]) {
    if (config->bpm < 300) {
      config->bpm++;
    }
  }

  if(buttons[RANK_BTN_BOTTOM]) {
    if (config->bpm > 30) {
      config->bpm--;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("BPM ");
  lcd.setCursor(16 - log10(config->bpm), 1);
  lcd.print(config->bpm);
}

/**
 * Udate Rate
 */
void updateRate(Configuration * config, Melody * melody) {
  // Manage update 
  float rates[] = { 1, 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125 };
  int rank = -1;
  for(int i=0; i < 8; i++) {
    if(rates[i] == config->rate) {
      rank = i;
      break;
    }
  }

  // Manage  buttons
  if(buttons[RANK_BTN_TOP]) {
    config->rate = rates[(rank + 1) % 8];
  }

  if(buttons[RANK_BTN_BOTTOM]) {
    rank - 1 >= 0 
      ? config->rate = rates[(rank - 1) % 8]
      : config->rate = rates[7];
  }

  int ratio = 1/config->rate;
  String letters = String("1/") + ratio; 

  // LCD
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Rate ");
  lcd.setCursor(16 - letters.length(), 1);
  lcd.print(letters);
}

void setupLCD() {
  lcd.begin(16, 2);
  
  pinMode(PIN_BTN_TOP, INPUT_PULLUP);
  pinMode(PIN_BTN_TOP_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_BOTTOM_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_BOTTOM, INPUT_PULLUP);
  pinMode(PIN_BTN_BOTTOM_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_TOP_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_MIDDLE, INPUT_PULLUP);
}


void loopLCD(uint32_t time, Configuration * config, Melody * melody) {
  if (time % 100 < config->tick) {
    // clearScreen();

    updateButtonValue(PIN_BTN_TOP, RANK_BTN_TOP);
    updateButtonValue(PIN_BTN_TOP_RIGHT, RANK_BTN_TOP_RIGHT);
    updateButtonValue(PIN_BTN_BOTTOM_RIGHT, RANK_BTN_BOTTOM_RIGHT);
    updateButtonValue(PIN_BTN_BOTTOM, RANK_BTN_BOTTOM);
    updateButtonValue(PIN_BTN_BOTTOM_LEFT, RANK_BTN_BOTTOM_LEFT);
    updateButtonValue(PIN_BTN_TOP_LEFT, RANK_BTN_TOP_LEFT);
    updateButtonValue(PIN_BTN_MIDDLE, RANK_BTN_MIDDLE);

    switch(currentMode) {
      case MODE_BPM: updateBpm(config); break;
      case MODE_TONE: updateTone(config, melody); break;
      case MODE_RATE: updateRate(config, melody); break;
      case MODE_RYTHM: updateBpm(config); break;
      case MODE_MIDI: updateBpm(config); break;
    }
  }

  if(time % 150 < config->tick) {
    if(buttons[RANK_BTN_MIDDLE]) {
      currentMode = (currentMode + 1) % 5;
    }
  }
}




