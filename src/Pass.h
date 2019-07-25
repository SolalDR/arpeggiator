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
    void debug(bool recursive = false);
};

/**
 * Une phrase joué
 */
class Pass {
  public:
    PassNote * noteHead = NULL;
    int notesLenght = 0;
    int direction = 0;
    int variation = 0;
    int rank = -1;

    void clear();
    void debug(bool debugNotes = false);
    void addNote(int degree, int octave);
    PassNote * getPassNoteAt(int rank);
};

#endif