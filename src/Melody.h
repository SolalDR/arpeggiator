#ifndef Melody_h
#define Melody_h

/*
 * Une note seule
 */
class PassNote {
  public: 
    int degree;
    int octave;
    PassNote * next = NULL;
};

/*
 * Une touche
 */
class InputNode {
  public:
    int degree = -1;
    InputNode * next = NULL;
};

/**
 * Une phrase joué
 */
class Pass {
  public:
    PassNote * noteHead;
    Pass * next = NULL;
};

class Melody {
  public:
    int inputLength = 0;
    int fundamental;
    int octave;
    int variation;
    int mode;

    InputNode* inputHead = new InputNode();
    Pass* passHead = 0;
    void addDegree(int degree);
    void removeDegree(int degree);
    void debug();
};

#endif