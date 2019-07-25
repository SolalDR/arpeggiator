#ifndef Melody_h
#define Melody_h

#include "constants.h"
#include "Pass.h"

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
    Pass* computeNextPass(Pass * currentPass);
    int getMidiNote(PassNote * note);
    InputNode * getInputAt(int rank);
};

#endif