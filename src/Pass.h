#ifndef Pass_h
#include <arduino.h>
#define Pass_h

/*
 * Une note seule
 */
class PassNote {
  public: 
    int degree;
    int octave;
    PassNote * next = NULL;
    void clear();
    void debug(bool recursive);
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

    void addNote(int degree, int octave);
    Pass * createNextPass();
    PassNote * getPassNoteAt(int rank);
    void debug();
};

#endif