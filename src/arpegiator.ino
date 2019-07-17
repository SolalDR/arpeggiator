#include <stdlib.h>
#include <arduino.h>
#include "./constants.c"

#define DEBUG false
NoteStack * noteStack;
NoteStack * noteStack2;

/*
 * Clocking
 */
float time;
float lastTick;
float timeBetweenNote;
int bpm;
float tempo;

/*
 * Mode et fondamental
 */
int octave;
int fundamental; // 0=>A 1=>A# 3=>C
int mode[7];

/*
 * Inputs
 */
float inputLength;
float inputLengthRandom;
float inputVelocity;        // todo
float inputVelocityRandom;  // todo
int inputNotes[7];

/* 
 * Rythmic :
 * Le rythme représente la grille rythmique. 
 * Il dispose des information de vélocités et longueur des notes pour chaque item de la grille.
 */
int rythmicLength = 16;              // Longueur du tythme (nombre de tick avant de recommencer)
int rythmicVelocities[16];           // Toute les valeures de de vélocités pour le rythmic en cour
int rythmicLengths[16];              // Toute les longueurs de note pour le rythmic en cour
int rythmicPointer = 0;              // A quel index se trouve t'on dans le rythmic
int rythmicMode = PATTERN_LIVE;

/*
 * Mélody :
 * La mélodie représente l'ensemble des informations permettant de savoir quel note joué à un instant rythmique donné
 */
int melodyOctaveLenght = 0;           // Le nombre d'octave sur lequel l'ordre des notes est calculé
int melodyOctave = 0;
int melodyMode = 0;      // Méthode de parcour
int melodyPointer = 0;                // A quel index se trouve t'on dans la suite mélodique
int melodyIndex = 0;                  // A quel index se trouve t'on dans la liste des notes active
int melodyLength = 0;                 


/*
 * Quand on change le tempo ou le bpm. On met à jour le temps entre deux tick
 */
float getTimeBetweenNote() {
  return (((float) bpm/60.0) * tempo) * 1000.0;
}

/*
 * Quand l'ordre des notes ou le nombre de touche change
 * définis le temps de la mélodie (nombre de tick)
 */
void updateMelodyLength() {
  melodyLength = melodyMode == ASC_DESC_ORDER ? inputLength * 2 : inputLength;
}

/**
 * A chaque tick, incremente le pointer et fais avancer la mélodie
 * Reviens au début quand il arrive au bout de la mélodie
 */
void updateMelodyPointer() {
  melodyPointer = (melodyPointer + 1) % melodyLength;
}

/**
 * A chaque tick, incremente le pointer et fais avancer les octaves
 */
void updateMelodyOctave() {
  if(melodyOctaveLenght == 1) {
    melodyOctave = 0;
    return;
  }

  if(melodyMode == ASC_ORDER && melodyPointer == 0) {
    melodyOctave = (melodyOctave + 1) % melodyOctaveLenght;
  }

  if(melodyMode == DESC_ORDER && melodyPointer == 0) {
    melodyOctave = melodyOctave == 0 
      ? melodyOctaveLenght - 1 
      : melodyOctave - 1;
  }

  if(melodyMode == ASC_DESC_ORDER) {
    // todo
  } 

  if(melodyMode == CROSS_IN_ORDER) {
    // todo
  }

  if(melodyMode == CROSS_OUT_ORDER) { 
    // todo
  }

  if(melodyMode == RAND_ORDER) { 
    // todo 
  }
}


/* 
 * Récupère l'index dans le tableau d'input (note joué) en fonction du pointer 
 * et du mode de progression des notes
 */
int getInputIndex(int pointer) {  
  if (melodyMode == ASC_ORDER) return pointer;
  if (melodyMode == DESC_ORDER) return melodyLength - pointer - 1;
  if (melodyMode == ASC_DESC_ORDER) {
    if(pointer < melodyLength*melodyOctaveLenght/2) {
      return pointer;
    } else {
      int halfMelodyLength = melodyLength /2;
      return halfMelodyLength - (pointer - halfMelodyLength) - 1;
    }
  }
}

/**
 * Calcul la note midi depuis
 * - la note de base 21 => A0
 * - la fondamentale de l'accord courant
 * - l'octave de la fondamental
 * - l'octave dans la mélodie
 * - le mode harmonique et le degré de la note
 */
int getNoteFromRank(int rank) {
    return A0 + fundamental + octave*12 + melodyOctave*12 + mode[rank];
}

/*
 * Change de mode, copy un tableau de référence (ex: MODE_DO)
 * vers le mode utiliser 
 */
void updateMode(int mode[], int modeReference[]) {
  for(int i=0; i<7; i++) {
    mode[i] = modeReference[i];
  }
}


/**
 * Méthode d'initialisation
 */
void setup() {
  // Port série
  if (DEBUG) {
    Serial.begin(9600);
  } else {
    // MIDI baud rate:
    Serial.begin(31250); 
  }

  // Clocking
  time = millis();
  lastTick = -1000;
  bpm = 40;
  tempo = temp1_4;
  timeBetweenNote = getTimeBetweenNote();

  noteStack = new NoteStack();
  noteStack2 = new NoteStack();

  // Tonalité et mode
  octave = 3;      
  fundamental = 3; // DO
  int modeReference[] = MODE_DO;
  updateMode(mode, modeReference);

  // Longueur de note
  inputLength = 1.;
  inputLengthRandom = 0.;

  // Mélodie
  melodyOctaveLenght = 3;
  melodyMode = ASC_ORDER;

  // Pattern rythmique 
  // todo

  // Simule une triade en entrée
  inputLength = 3;
  inputNotes[0] = 0;
  inputNotes[1] = 2;
  inputNotes[2] = 4;
  updateMelodyLength();
}


void loop() {
  // Clocking
  float deltaTime = abs(time - lastTick);
  time = millis();
  if (deltaTime > timeBetweenNote) {
    // Compute next note
    updateMelodyPointer();
    updateMelodyOctave();
    melodyIndex = getInputIndex(melodyPointer);
    int midiNote = getNoteFromRank(inputNotes[melodyIndex]);

    // Compute duration
    float baseDuration = timeBetweenNote*inputLength;
    // to fix: may cause some  bug
    float randomDuration = inputLengthRandom*inputLength*random(0, 100)*timeBetweenNote;
    float duration = (float) baseDuration - randomDuration;

    noteStack->addNote(midiNote, 0x45, time + duration);
    noteStack->removeOldNotes();

    // update clocking clocking
    lastTick = time;
  }
  
  delay(4);
}