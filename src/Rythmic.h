#ifndef Rythmic_h
#define Rythmic_h
#include "arduino.h"
#include "constants.h"
#define LENGTH_MAX 64

class RythmicTick {
  public: 
    float duration = 1;
    float velocity = 1;
    RythmicTick* next = NULL;
};

/**
 * Singleton représentant la structure rythmique
 */
class Rythmic {
  private:
   int pointer;
   int length;
   RythmicTick * tmp;

  public:
    int mode;
    float tickBaseLength;
    float tickRandomLength;
    float tickBaseVelocity;
    float tickRandomVelocity;
    
    RythmicTick * ticks[LENGTH_MAX];
    Rythmic();
    RythmicTick * getTick();
    RythmicTick * computeTick();

    int advance();
    int setLength(int length);
    int getLength();
};

#endif