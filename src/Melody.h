#include "constants.h"
#ifndef Melody_h
#define Melody_h
#define DIR_ASC 1
#define DIR_DESC 2
#define DIR_ASC_DESC 3
#define DIR_IN 4
#define DIR_RAND 5
#define DIR_HAND 6



/*
 * Une note seule
 */
class PassNote {
  public: 
    int degree;
    int octave;
    PassNote * next = NULL;
    void clear();
};

/**
 * Une phrase joué
 */
class Pass {
  public:
    PassNote * noteHead = NULL;
    Pass * next = NULL;
    int notesLenght = 0;
    int direction = 0;
    int variation = 0;
    int rank = -1;
    void clear();
    void debug();
    void addNote(int degree, int octave);
    PassNote * getPassNoteAt(int rank);
};

/*
 * Une touche
 */
class InputNode {
  public:
    int degree = -1;
    InputNode * next = NULL;
};


class Melody {
  public:
    int inputLength = 0;
    int fundamental;
    int octave = 1;
    int octaveLength = 1;
    int variation = 1;
    int direction = DIR_ASC;
    int passNoteIndex = -1;
    
    int mode[7] = MODE_DO;
    InputNode* inputHead = new InputNode();
    Pass* passHead = NULL;

    void addDegree(int degree);
    void removeDegree(int degree);
    void debug();
    void updatePasses();
    int advance();
    void advancePass();
    // Pass* computeNextPass(Pass * currentPass);
    int getMidiNote(PassNote * note);
    InputNode * getInputAt(int rank);
};

#endif