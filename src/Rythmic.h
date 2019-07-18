#ifndef Rythmic_h

#define Rythmic_h
#define LENGTH_MAX 64
#define MODE_LIVE 1
#define MODE_GRID 2

#include "arduino.h"

class RythmicTick {
  public: 
    float duration = 1;
    float velocity = 1;
    RythmicTick* next = NULL;
};

class Rythmic {

  private:
   int pointer;
   int length;
   RythmicTick tmp;


  public:
    int mode;
    float tickBaseLength;
    float tickRandomLength;
    float tickBaseVelocity;
    float tickRandomVelocity;
    
    RythmicTick ticks[LENGTH_MAX];
    Rythmic();
    RythmicTick getTick();
    RythmicTick computeTick();

    int advance();
    int setLength(int length);
    int getLength();
};

#endif